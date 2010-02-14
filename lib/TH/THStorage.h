#ifndef TH_STORAGE_INC
#define TH_STORAGE_INC

#include "THGeneral.h"

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

#define THStorageAPI TH_CONCAT_3(TH,Real,StorageAPI)
#define THStorage    TH_CONCAT_3(TH,Real,Storage)

#define TH_GENERIC_FILE "THStorageGeneric.h"
#include "THGenerateAllTypes.h"

#endif
