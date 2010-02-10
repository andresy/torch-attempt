#define real unsigned char
#define THStorage THByteStorage
#define THStorageAPI THByteStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real char
#define THStorage THCharStorage
#define THStorageAPI THCharStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real short
#define THStorage THShortStorage
#define THStorageAPI THShortStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real int
#define THStorage THIntStorage
#define THStorageAPI THIntStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real long
#define THStorage THLongStorage
#define THStorageAPI THLongStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real float
#define THStorage THFloatStorage
#define THStorageAPI THFloatStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI

#define real double
#define THStorage THDoubleStorage
#define THStorageAPI THDoubleStorageAPI
#include "THStorage.h"
#undef real
#undef THStorage
#undef THStorageAPI
