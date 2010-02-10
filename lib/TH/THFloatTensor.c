#include "THTensors.h"

#define real float
#define THStorage THFloatStorage
#define THStorageAPI THFloatStorageAPI
#define THTensor THFloatTensor
#define THTensorAPI THFloatTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
