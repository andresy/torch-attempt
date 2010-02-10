#include "THTensors.h"

#define real int
#define THStorage THIntStorage
#define THStorageAPI THIntStorageAPI
#define THTensor THIntTensor
#define THTensorAPI THIntTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
