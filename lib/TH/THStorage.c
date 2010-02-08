#include "THStorage.h"

/* stuff for mapped files */
#ifdef _WIN32
#include <windows.h>
#endif

#if HAVE_MMAP
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
/* end of stuff for mapped files */

/* on pourrait peut-etre mettre ca dans TH/dev/THStorage.h */
struct THStorageVTable
{
  real* (*data)(THStorage*);
  long (*size)(THStorage*);

  void (*retain)(THStorage*);
  void (*copy)(THStorage*, THStorage*);
  void (*resize)(THStorage*, long);
  
  void (*free)(THStorage*);
};

struct THStorage
{
    struct THStorageVTable *vtable;
    long size;
    int refcount;
    real *data;
};

/* normal storages */
static struct THStorageVTable THStorage_vtable;

static THStorage* THStorage_alloc()
{
  THStorage *self = malloc(sizeof(THStorage));
  self->vtable = &THStorage_vtable;
  self->size = 0;
  self->refcount = 1;
  self->data = NULL;
  return self;
}

static THStorage* THStorage_allocWithSize(long size)
{
  THStorage *self = THStorage_alloc();
  self->size = size;
  self->data = malloc(sizeof(real)*size);
  return self;
}

static real* THStorage_data(THStorage *self)
{
  return self->data;
}

static long THStorage_size(THStorage *self)
{
  return self->size;
}

static void THStorage_resize(THStorage *self, long size)
{
  if(size != self->size)
  {
    self->size = size;
    self->data = realloc(self->data, sizeof(real)*size);
  }
}

static void THStorage_copy(THStorage *self, THStorage *storage)
{
  int i;

/*  THAssert(self->size == self->size); */
  
  for(i = 0; i < self->size; i++)
    self->data[i] = self->data[i];
}

static void THStorage_retain(THStorage *self)
{
  ++self->refcount;
}

static void THStorage_free(THStorage *self)
{
  if(--self->refcount == 0)
  {
    free(self->data);
    free(self);
  }
}

static struct THStorageVTable THStorage_vtable = {
  THStorage_data,
  THStorage_size,
  THStorage_retain,
  THStorage_copy,
  THStorage_resize,
  THStorage_free
};

/* mapped storages */

static struct THStorageVTable THMappedStorage_vtable;

#if defined(_WIN32) || defined(HAVE_MMAP)

static THStorage* THStorage_allocWithMapping(const char *filename, int isshared)
{
  THStorage *self = malloc(sizeof(THStorage));
  long size;

  /* virtual table */
  self->vtable = &THMappedStorage_vtable;

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

static THStorage* THStorage_allocWithMapping(const char *filename, int isshared)
{
  THError("File mapped storages are not supported on your system");
  return NULL;
}

#endif

static void THMappedStorage_resize(THStorage *self, long size)
{
  THError("File mapped storages cannot be resized");
}

static void THMappedStorage_free(THStorage *self)
{
#if defined(_WIN32) || defined(HAVE_MMAP)
  if(--self->refcount == 0)
  {
#ifdef _WIN32
    if(!UnmapViewOfFile((LPINT)self->data))
#else
    if (munmap(self->data, self->size*sizeof(real)))
#endif
      THError("could not unmap the shared memory file");
  }
#endif
}

#if defined(_WIN32) || defined(HAVE_MMAP)

static struct THStorageVTable THMappedStorage_vtable = {
  THStorage_data,
  THStorage_size,
  THStorage_retain,
  THStorage_copy,
  THMappedStorage_resize,
  THMappedStorage_free
};

#endif

/* ze vtable */
static real* THStorage_vtable_data(THStorage *self)
{
  return self->vtable->data(self);
}

static long THStorage_vtable_size(THStorage *self)
{
  return self->vtable->size(self);
}

static void THStorage_vtable_retain(THStorage *self)
{
  self->vtable->retain(self);
}

static void THStorage_vtable_copy(THStorage *self, THStorage *src)
{
  self->vtable->copy(self, src);
}

static void THStorage_vtable_resize(THStorage *self, long size)
{
  self->vtable->resize(self, size);
}

static void THStorage_vtable_free(THStorage *self)
{
  self->vtable->free(self);
}

const struct THMStorage THMStorage = {THStorage_alloc,
                                      THStorage_allocWithSize,
                                      THStorage_allocWithMapping,
                                      THStorage_vtable_data,
                                      THStorage_vtable_size,
                                      THStorage_vtable_retain,
                                      THStorage_vtable_copy,
                                      THStorage_vtable_resize,
                                      THStorage_vtable_free
};
