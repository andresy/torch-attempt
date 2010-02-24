#include "TensorMath.h"

static const void *torch_ByteTensor_id = NULL;
static const void *torch_CharTensor_id = NULL;
static const void *torch_ShortTensor_id = NULL;
static const void *torch_IntTensor_id = NULL;
static const void *torch_LongTensor_id = NULL;
static const void *torch_FloatTensor_id = NULL;
static const void *torch_DoubleTensor_id = NULL;

#include "generic/TensorMath.c"
#include "THGenerateAllTypes.h"
