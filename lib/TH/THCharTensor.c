#include "THTensors.h"

#define real char
#define THStorage THCharStorage
#define THStorageAPI THCharStorageAPI
#define THTensor THCharTensor
#define THTensorAPI THCharTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
