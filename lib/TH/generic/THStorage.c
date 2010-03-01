#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THStorage.c"
#else

struct THStorage
{
    long size;
    int refcount;
    real *data;
    int ismapped;
};

THStorage* THStorage_(new)(void)
{
  THStorage *self = THAlloc(sizeof(THStorage));
  self->size = 0;
  self->refcount = 1;
  self->data = NULL;
  self->ismapped = 0;
  return self;
}

THStorage* THStorage_(newWithSize)(long size)
{
  THStorage *self = THStorage_(new)();
  self->size = size;
  self->data = THAlloc(sizeof(real)*size);
  return self;
}

real* THStorage_(data)(THStorage *self)
{
  return self->data;
}

long THStorage_(size)(THStorage *self)
{
  return self->size;
}

void THStorage_(fill)(THStorage *self, real value)
{
  long i;
  for(i = 0; i < self->size; i++)
    self->data[i] = value;
}

void THStorage_(resize)(THStorage *self, long size)
{
  if(self->ismapped)
    THError("File mapped storages cannot be resized");

  if(size != self->size)
  {
    self->size = size;
    self->data = THRealloc(self->data, sizeof(real)*size);
  }
}

void THStorage_(copy)(THStorage *self, THStorage *storage)
{
  int i;

  THArgCheck(self->size == storage->size, 2, "inconsistent storage sizes");
  
  for(i = 0; i < self->size; i++)
    self->data[i] = storage->data[i];
}

void THStorage_(retain)(THStorage *self)
{
  ++self->refcount;
}

real THStorage_(get)(THStorage *self, long idx)
{
  THArgCheck(idx >= 0 && idx < self->size, 2, "index out of range");
  return self->data[idx];
}

void THStorage_(set)(THStorage *self, long idx, real value)
{
  THArgCheck(idx >= 0 && idx < self->size, 2, "index out of range");
  self->data[idx] = value;
}

void THStorage_(free)(THStorage *self)
{
  if(--self->refcount == 0)
  {
#if defined(_WIN32) || defined(HAVE_MMAP)
    if(self->ismapped)
    {
#ifdef _WIN32
      if(!UnmapViewOfFile((LPINT)self->data))
#else
      if (munmap(self->data, self->size*sizeof(real)))
#endif
          THError("could not unmap the shared memory file");
    }
    else
#endif
    {
      free(self->data);
    }
    free(self);
  }
}

/* mapped storages */
#if defined(_WIN32) || defined(HAVE_MMAP)

THStorage* THStorage_(newWithMapping)(const char *filename, int isshared)
{
  THStorage *self = THAlloc(sizeof(THStorage));
  long size;

  /* check size */
  FILE *f = fopen(filename, "rb");
  if(f == NULL)
    THError("unable to open file <%s> for mapping (read-only mode)", filename);
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fclose(f);
  size /= sizeof(real);

#ifdef _WIN32
  {
    HANDLE hfile;
    HANDLE hmfile;
    DWORD size_hi, size_lo;

    /* open file */
    if(isshared)
    {
      hfile = CreateFileA(filename, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
      if (hfile == INVALID_HANDLE_VALUE)
        THError("could not open file <%s> in read-write mode", filename);
    }
    else
    {
      hfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
      if (hfile == INVALID_HANDLE_VALUE)
        THError("could not open file <%s> in read-only mode", filename);
    }

#if SIZEOF_SIZE_T > 4
    size_hi = (DWORD)((size*sizeof(real)) >> 32);
    size_lo = (DWORD)((size*sizeof(real)) & 0xFFFFFFFF);
#else
    size_hi = 0;
    size_lo = (DWORD)(size*sizeof(real));
#endif

    /* get map handle */
    if(isshared)
    {
      if( (hmfile = CreateFileMapping(hfile, NULL, PAGE_READWRITE, size_hi, size_lo, NULL)) == NULL )
        THError("could not create a map on file <%s>", filename);
    }
    else
    {
      if( (hmfile = CreateFileMapping(hfile, NULL, PAGE_WRITECOPY, size_hi, size_lo, NULL)) == NULL )
        THError("could not create a map on file <%s>", filename);
    }

    /* map the stuff */
    if(isshared)
      self->data = MapViewOfFile(hmfile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    else
      self->data = MapViewOfFile(hmfile, FILE_MAP_COPY, 0, 0, 0);
      
    self->size = size;
    if(self->data == NULL)
    {
      free(storage);
      THError("memory map failed on file <%s>", filename);
    }
    CloseHandle(hfile); 
    CloseHandle(hmfile); 
  }
#else
  {
    /* open file */
    int fd;
    if(isshared)
    {
      fd = open(filename, O_RDWR);
      if(fd == -1)
        THError("unable to open file <%s> in read-write mode", filename);
    }
    else
    {
      fd = open(filename, O_RDONLY);
      if(fd == -1)
        THError("unable to open file <%s> in read-only mode", filename);
    }
    
    /* map it */
    if(isshared)
      self->data = mmap(NULL, size*sizeof(real), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    else
      self->data = mmap(NULL, size*sizeof(real), PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

    self->size = size;
    if(self->data == MAP_FAILED)
    {
      self->data = NULL; /* let's be sure it is NULL before calling free() */
      free(self);
      THError("memory map failed on file <%s>", filename);
    }
    close (fd);
  }
#endif

  self->refcount = 1;
  return self;
}

#else

THStorage* THStorage_(newWithMapping)(const char *filename, int isshared)
{
  THError("File mapped storages are not supported on your system");
  return NULL;
}

#endif

#endif
