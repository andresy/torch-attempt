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

#define THStorage        TH_CONCAT_3(TH,Real,Storage)
#define THStorage_(NAME) TH_CONCAT_4(TH,Real,Storage_,NAME)

#include "generic/THStorage.h"
#include "THGenerateAllTypes.h"

#endif
