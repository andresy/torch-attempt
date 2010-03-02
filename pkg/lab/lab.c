#include "TH.h"
#include "luaT.h"
#include "utils.h"

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_string_(NAME) TH_CONCAT_STRING_3(torch., Real, NAME)

#define lab_(NAME) TH_CONCAT_3(lab_, Real, NAME)

#include "generic/lab.c"
#include "THGenerateFloatTypes.h"
