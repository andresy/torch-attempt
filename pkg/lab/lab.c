#include "TH.h"
#include "luaT.h"
#include "utils.h"

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_string_(NAME) TH_CONCAT_STRING_3(torch., Real, NAME)

#define lab_(NAME) TH_CONCAT_3(lab_, Real, NAME)

static const void* torch_ByteTensor_id;
static const void* torch_CharTensor_id;
static const void* torch_ShortTensor_id;
static const void* torch_IntTensor_id;
static const void* torch_LongTensor_id;
static const void* torch_FloatTensor_id;
static const void* torch_DoubleTensor_id;

static const void* torch_LongStorage_id;

#include "generic/lab.c"
#include "THGenerateAllTypes.h"
