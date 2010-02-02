
#if defined(_WIN32) || defined(HAVE_MMAP)

template <typename T>* Storage<T>* Storage_newWithMapping(const char *fileName, int isShared)
{
  Storage<T> *storage = new Storage<T>;
  long size;

  /* check size */
  FILE *f = fopen(fileName, "rb");
  if(f == NULL)
    THError("unable to open file <%s> for mapping (read-only mode)", fileName);
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fclose(f);
  size /= sizeof(TYPE);

#ifdef _WIN32
  {
    HANDLE hfile;
    HANDLE hmfile;
    DWORD size_hi, size_lo;

    /* open file */
    if(isShared)
    {
      hfile = CreateFileA(fileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
      if (hfile == INVALID_HANDLE_VALUE)
        THError("could not open file <%s> in read-write mode", fileName);
    }
    else
    {
      hfile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
      if (hfile == INVALID_HANDLE_VALUE)
        THError("could not open file <%s> in read-only mode", fileName);
    }

#if SIZEOF_SIZE_T > 4
    size_hi = (DWORD)((size*sizeof(TYPE)) >> 32);
    size_lo = (DWORD)((size*sizeof(TYPE)) & 0xFFFFFFFF);
#else
    size_hi = 0;
    size_lo = (DWORD)(size*sizeof(TYPE));
#endif

    /* get map handle */
    if(isShared)
    {
      if( (hmfile = CreateFileMapping(hfile, NULL, PAGE_READWRITE, size_hi, size_lo, NULL)) == NULL )
        THError("could not create a map on file <%s>", fileName);
    }
    else
    {
      if( (hmfile = CreateFileMapping(hfile, NULL, PAGE_WRITECOPY, size_hi, size_lo, NULL)) == NULL )
        THError("could not create a map on file <%s>", fileName);
    }

    /* map the stuff */
    storage = STORAGE_FUNC(new)();
    if(isShared)
      storage->data = MapViewOfFile(hmfile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    else
      storage->data = MapViewOfFile(hmfile, FILE_MAP_COPY, 0, 0, 0);
      
    storage->size = size;
    if(storage->data == NULL)
    {
      STORAGE_FUNC(free)(storage);
      THError("memory map failed on file <%s>", fileName);
    }
    CloseHandle(hfile); 
    CloseHandle(hmfile); 
  }
#else
  {
    /* open file */
    int fd;
    if(isShared)
    {
      fd = open(fileName, O_RDWR);
      if(fd == -1)
        THError("unable to open file <%s> in read-write mode", fileName);
    }
    else
    {
      fd = open(fileName, O_RDONLY);
      if(fd == -1)
        THError("unable to open file <%s> in read-only mode", fileName);
    }
    
    /* map it */
    storage = STORAGE_FUNC(new)();
    if(isShared)
      storage->data = mmap(NULL, size*sizeof(TYPE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    else
      storage->data = mmap(NULL, size*sizeof(TYPE), PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

    storage->size = size;
    if(storage->data == MAP_FAILED)
    {
      storage->data = NULL; /* let's be sure it is NULL before calling free() */
      STORAGE_FUNC(free)(storage);
      THError("memory map failed on file <%s>", fileName);
    }
    close (fd);
  }
#endif

  storage->refcount = 1;
  storage->isMapped = 1;
  return storage;
}

#else

STORAGE* STORAGE_FUNC(newWithMapping)(const char *fileName, int isShared)
{
  THError("Mapped file Storages are not supported on your system");
}

#endif

#if defined(_WIN32) || defined(HAVE_MMAP)
      if(storage->isMapped)
      {
#ifdef _WIN32
        if(!UnmapViewOfFile((LPINT)storage->data))
#else
        if (munmap(storage->data, storage->size*sizeof(TYPE)))
#endif
          THError("could not unmap the shared memory file");
      }
      else
#endif
