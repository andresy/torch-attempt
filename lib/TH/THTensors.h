#include "THStorages.h"

#define real unsigned char
#define THStorage THByteStorage
#define THStorageAPI THByteStorageAPI
#define THTensor THByteTensor
#define THTensorAPI THByteTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real char
#define THStorage THCharStorage
#define THStorageAPI THCharStorageAPI
#define THTensor THCharTensor
#define THTensorAPI THCharTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real short
#define THStorage THShortStorage
#define THStorageAPI THShortStorageAPI
#define THTensor THShortTensor
#define THTensorAPI THShortTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real int
#define THStorage THIntStorage
#define THStorageAPI THIntStorageAPI
#define THTensor THIntTensor
#define THTensorAPI THIntTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real long
#define THStorage THLongStorage
#define THStorageAPI THLongStorageAPI
#define THTensor THLongTensor
#define THTensorAPI THLongTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real float
#define THStorage THFloatStorage
#define THStorageAPI THFloatStorageAPI
#define THTensor THFloatTensor
#define THTensorAPI THFloatTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI

#define real double
#define THStorage THDoubleStorage
#define THStorageAPI THDoubleStorageAPI
#define THTensor THDoubleTensor
#define THTensorAPI THDoubleTensorAPI
#include "THTensor.h"
#undef real
#undef THStorage
#undef THStorageAPI
#undef THTensor
#undef THTensorAPI
