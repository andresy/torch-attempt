#include "THTensors.h"

#define real double
#define THStorage THDoubleStorage
#define THStorageAPI THDoubleStorageAPI
#define THTensor THDoubleTensor
#define THTensorAPI THDoubleTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
