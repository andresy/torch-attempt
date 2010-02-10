#include "THTensors.h"

#define real unsigned char
#define THStorage THByteStorage
#define THStorageAPI THByteStorageAPI
#define THTensor THByteTensor
#define THTensorAPI THByteTensorAPI
#include "THTensor.c"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
