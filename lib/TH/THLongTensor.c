#include "THTensors.h"

#define real long
#define THStorage THLongStorage
#define THStorageAPI THLongStorageAPI
#define THTensor THLongTensor
#define THTensorAPI THLongTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
