#include "Tensor.h"
#include "THTensor.h"
#include "THTensorApply.h"

static const void *torch_ByteStorage_id = NULL;
static const void *torch_CharStorage_id = NULL;
static const void *torch_ShortStorage_id = NULL;
static const void *torch_IntStorage_id = NULL;
static const void *torch_LongStorage_id = NULL;
static const void *torch_FloatStorage_id = NULL;
static const void *torch_DoubleStorage_id = NULL;

static const void *torch_ByteTensor_id = NULL;
static const void *torch_CharTensor_id = NULL;
static const void *torch_ShortTensor_id = NULL;
static const void *torch_IntTensor_id = NULL;
static const void *torch_LongTensor_id = NULL;
static const void *torch_FloatTensor_id = NULL;
static const void *torch_DoubleTensor_id = NULL;

#include "generic/Tensor.c"
#include "THGenerateAllTypes.h"
