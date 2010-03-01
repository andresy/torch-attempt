#include "THStorage.h"

#define THStorageVTable           TH_CONCAT_3(TH,Real,StorageVTable)

#define THMappedStorage           TH_CONCAT_3(TH,Real,MappedStorage)
#define THMappedStorage_(NAME)    TH_CONCAT_4(TH,Real,MappedStorage_,NAME)

#include "generic/THStorage.c"
#include "THGenerateAllTypes.h"
