#include "THTensors.h"

#define real short
#define THStorage THShortStorage
#define THStorageAPI THShortStorageAPI
#define THTensor THShortTensor
#define THTensorAPI THShortTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
