#ifndef TORCH_GENERAL_INC
#define TORCH_GENERAL_INC

#include <stdlib.h>
#include <string.h>

#include "luaT.h"
#include "TH.h"

#ifdef _MSC_VER

#define snprintf _snprintf
#define popen _popen
#define pclose _pclose

#endif

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_string_(NAME) TH_CONCAT_STRING_3(torch., Real, NAME)

#endif
