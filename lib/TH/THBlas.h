#ifndef TH_BLAS_INC
#define TH_BLAS_INC

#include "THGeneral.h"

#define THBlasAPI    TH_CONCAT_3(TH,Real,BlasAPI)
#define THBlas_(NAME) TH_CONCAT_4(TH,Real,Blas_,NAME)

#define TH_GENERIC_FILE "THBlasGeneric.h"
#include "THGenerateAllTypes.h"

#endif
