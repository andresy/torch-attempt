#include "THStorage.h"

#define THStorageVTable     TH_CONCAT_3(TH,Real,StorageVTable)
#define THStorage_(NAME)    TH_CONCAT_4(TH,Real,Storage_,NAME)

#define THMappedStorage           TH_CONCAT_3(TH,Real,MappedStorage)
#define THMappedStorage_(NAME)    TH_CONCAT_4(TH,Real,MappedStorage_,NAME)

#define TH_GENERIC_FILE "THStorageGeneric.c"
#include "THGenerateAllTypes.h"
