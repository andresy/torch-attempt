#ifndef TH_TENSOR_DIM_APPLY_INC
#define TH_TENSOR_DIM_APPLY_INC

#define TH_GEN_TENSOR_DIM_APPLY3(TENSOR1, TENSOR2, TENSOR3, DIMENSION, CODE) \
  TH_TENSOR_DIM_APPLY3(real, Real, TENSOR1, real, Real, TENSOR2, real, Real, TENSOR3, DIMENSION, CODE)

#define TH_GEN_TENSOR_DIM_APPLY2(TENSOR1, TENSOR2, DIMENSION, CODE) \
  TH_TENSOR_DIM_APPLY2(real, Real, TENSOR1, real, Real, TENSOR2, DIMENSION, CODE)

#define TH_GEN_TENSOR_DIM_APPLY(TENSOR1, DIMENSION, CODE) \
  TH_TENSOR_DIM_APPLY(real, Real, TENSOR1, DIMENSION, CODE)

#define TH_TENSOR_DIM_APPLY3(TYPE1, CTYPE1, TENSOR1, TYPE2, CTYPE2, TENSOR2, TYPE3, CTYPE3, TENSOR3, DIMENSION, CODE) \
  {                                                                     \
    TYPE1 *TENSOR1##_data = NULL;                                       \
    long TENSOR1##_stride = 0, TENSOR1##_size = 0;                      \
    TYPE2 *TENSOR2##_data = NULL;                                       \
    long TENSOR2##_stride = 0, TENSOR2##_size = 0;                      \
    TYPE3 *TENSOR3##_data = NULL;                                       \
    long TENSOR3##_stride = 0, TENSOR3##_size = 0;                      \
    long *TH_TENSOR_DIM_APPLY_counter = NULL;                           \
    int TH_TENSOR_DIM_APPLY_hasFinished = 0;                            \
    int TH_TENSOR_DIM_APPLY_i;                                          \
                                                                        \
    if( (DIMENSION < 0) || (DIMENSION >= TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)) ) \
      THError("invalid dimension");                                     \
    if( TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1) != TH_CONCAT_4(TH, CTYPE2, Tensor_, nDimension)(TENSOR2) ) \
      THError("inconsistent tensor sizes");                             \
    if( TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1) != TH_CONCAT_4(TH, CTYPE3, Tensor_, nDimension)(TENSOR3) ) \
      THError("inconsistent tensor sizes");                             \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
    {                                                                   \
      if(TH_TENSOR_DIM_APPLY_i == DIMENSION)                            \
        continue;                                                       \
      if(TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, TH_TENSOR_DIM_APPLY_i) != TH_CONCAT_4(TH, CTYPE2, Tensor_, size)(TENSOR2, TH_TENSOR_DIM_APPLY_i)) \
        THError("inconsistent tensor sizes");                           \
      if(TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, TH_TENSOR_DIM_APPLY_i) != TH_CONCAT_4(TH, CTYPE3, Tensor_, size)(TENSOR3, TH_TENSOR_DIM_APPLY_i)) \
        THError("inconsistent tensor sizes");                           \
    }                                                                   \
                                                                        \
    TH_TENSOR_DIM_APPLY_counter = (long*)THAlloc(sizeof(long)*(TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1))); \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;           \
                                                                        \
    TENSOR1##_data = TH_CONCAT_4(TH, CTYPE1, Tensor_, data)(TENSOR1);                  \
    TENSOR1##_stride = TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, DIMENSION);   \
    TENSOR1##_size = TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, DIMENSION);       \
                                                                        \
    TENSOR2##_data = TH_CONCAT_4(TH, CTYPE2, Tensor_, data)(TENSOR2);                  \
    TENSOR2##_stride = TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, DIMENSION);   \
    TENSOR2##_size = TH_CONCAT_4(TH, CTYPE2, Tensor_, size)(TENSOR2, DIMENSION);       \
                                                                        \
    TENSOR3##_data = TH_CONCAT_4(TH, CTYPE3, Tensor_, data)(TENSOR3);                  \
    TENSOR3##_stride = TH_CONCAT_4(TH, CTYPE3, Tensor_, stride)(TENSOR3, DIMENSION);   \
    TENSOR3##_size = TH_CONCAT_4(TH, CTYPE3, Tensor_, size)(TENSOR3, DIMENSION);       \
                                                                        \
    while(!TH_TENSOR_DIM_APPLY_hasFinished)                             \
    {                                                                   \
      CODE                                                              \
                                                                        \
      if(TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1) == 1)                   \
        break;                                                          \
                                                                        \
      for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
      {                                                                 \
        if(TH_TENSOR_DIM_APPLY_i == DIMENSION)                          \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          continue;                                                     \
        }                                                               \
                                                                        \
        TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++;           \
        TENSOR1##_data += TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, TH_TENSOR_DIM_APPLY_i); \
        TENSOR2##_data += TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, TH_TENSOR_DIM_APPLY_i); \
        TENSOR3##_data += TH_CONCAT_4(TH, CTYPE3, Tensor_, stride)(TENSOR3, TH_TENSOR_DIM_APPLY_i); \
                                                                        \
        if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, TH_TENSOR_DIM_APPLY_i)) \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          else                                                          \
          {                                                             \
            TENSOR1##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, TH_TENSOR_DIM_APPLY_i); \
            TENSOR2##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, TH_TENSOR_DIM_APPLY_i); \
            TENSOR3##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH_CONCAT_4(TH, CTYPE3, Tensor_, stride)(TENSOR3, TH_TENSOR_DIM_APPLY_i); \
            TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;     \
          }                                                             \
        }                                                               \
        else                                                            \
          break;                                                        \
      }                                                                 \
    }                                                                   \
    THFree(TH_TENSOR_DIM_APPLY_counter);                                \
  }

#define TH_TENSOR_DIM_APPLY2(TYPE1, CTYPE1, TENSOR1, TYPE2, CTYPE2, TENSOR2, DIMENSION, CODE) \
  {                                                                     \
    TYPE1 *TENSOR1##_data = NULL;                                       \
    long TENSOR1##_stride = 0, TENSOR1##_size = 0;                      \
    TYPE2 *TENSOR2##_data = NULL;                                       \
    long TENSOR2##_stride = 0, TENSOR2##_size = 0;                      \
    long *TH_TENSOR_DIM_APPLY_counter = NULL;                           \
    int TH_TENSOR_DIM_APPLY_hasFinished = 0;                            \
    int TH_TENSOR_DIM_APPLY_i;                                          \
                                                                        \
    if( (DIMENSION < 0) || (DIMENSION >= TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)) ) \
      THError("invalid dimension");                                     \
    if( TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1) != TH_CONCAT_4(TH, CTYPE2, Tensor_, nDimension)(TENSOR2) ) \
      THError("inconsistent tensor sizes");                             \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
    {                                                                   \
      if(TH_TENSOR_DIM_APPLY_i == DIMENSION)                            \
        continue;                                                       \
      if(TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, TH_TENSOR_DIM_APPLY_i) != TH_CONCAT_4(TH, CTYPE2, Tensor_, size)(TENSOR2, TH_TENSOR_DIM_APPLY_i)) \
        THError("inconsistent tensor sizes");                           \
    }                                                                   \
                                                                        \
    TH_TENSOR_DIM_APPLY_counter = (long*)THAlloc(sizeof(long)*(TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1))); \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;           \
                                                                        \
    TENSOR1##_data = TH_CONCAT_4(TH, CTYPE1, Tensor_, data)(TENSOR1);                  \
    TENSOR1##_stride = TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, DIMENSION);   \
    TENSOR1##_size = TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, DIMENSION);       \
                                                                        \
    TENSOR2##_data = TH_CONCAT_4(TH, CTYPE2, Tensor_, data)(TENSOR2);                  \
    TENSOR2##_stride = TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, DIMENSION);   \
    TENSOR2##_size = TH_CONCAT_4(TH, CTYPE2, Tensor_, size)(TENSOR2, DIMENSION);       \
                                                                        \
    while(!TH_TENSOR_DIM_APPLY_hasFinished)                             \
    {                                                                   \
      CODE                                                              \
                                                                        \
      if(TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1) == 1)                   \
        break;                                                          \
                                                                        \
      for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1); TH_TENSOR_DIM_APPLY_i++) \
      {                                                                 \
        if(TH_TENSOR_DIM_APPLY_i == DIMENSION)                          \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          continue;                                                     \
        }                                                               \
                                                                        \
        TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++;           \
        TENSOR1##_data += TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, TH_TENSOR_DIM_APPLY_i); \
        TENSOR2##_data += TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, TH_TENSOR_DIM_APPLY_i); \
                                                                        \
        if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TH_CONCAT_4(TH, CTYPE1, Tensor_, size)(TENSOR1, TH_TENSOR_DIM_APPLY_i)) \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH_CONCAT_4(TH, CTYPE1, Tensor_, nDimension)(TENSOR1)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          else                                                          \
          {                                                             \
            TENSOR1##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH_CONCAT_4(TH, CTYPE1, Tensor_, stride)(TENSOR1, TH_TENSOR_DIM_APPLY_i); \
            TENSOR2##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH_CONCAT_4(TH, CTYPE2, Tensor_, stride)(TENSOR2, TH_TENSOR_DIM_APPLY_i); \
            TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;     \
          }                                                             \
        }                                                               \
        else                                                            \
          break;                                                        \
      }                                                                 \
    }                                                                   \
    THFree(TH_TENSOR_DIM_APPLY_counter);                                \
  }

#define TH_TENSOR_DIM_APPLY(TYPE, CTYPE, TENSOR, DIMENSION, CODE)       \
  {                                                                     \
    TYPE *TENSOR##_data = NULL;                                         \
    long TENSOR##_stride = 0, TENSOR##_size = 0;                        \
    long *TH_TENSOR_DIM_APPLY_counter = NULL;                           \
    int TH_TENSOR_DIM_APPLY_hasFinished = 0;                            \
    int TH_TENSOR_DIM_APPLY_i;                                          \
                                                                        \
    if( (DIMENSION < 0) || (DIMENSION >= TH##CTYPE##Tensor_nDimension(TENSOR)) ) \
      THError("invalid dimension");                                     \
                                                                        \
    TENSOR##_data = TH##CTYPE##Tensor_data(TENSOR);                     \
    TENSOR##_stride = TH##CTYPE##Tensor_stride(TENSOR, DIMENSION);      \
    TENSOR##_size = TH##CTYPE##Tensor_size(TENSOR, DIMENSION);          \
    TH_TENSOR_DIM_APPLY_counter = (long*)THAlloc(sizeof(long)*(TH##CTYPE##Tensor_nDimension(TENSOR))); \
    for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH##CTYPE##Tensor_nDimension(TENSOR); TH_TENSOR_DIM_APPLY_i++) \
      TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;           \
                                                                        \
    while(!TH_TENSOR_DIM_APPLY_hasFinished)                             \
    {                                                                   \
      CODE                                                              \
                                                                        \
      if(TH##CTYPE##Tensor_nDimension(TENSOR) == 1)                     \
        break;                                                          \
                                                                        \
      for(TH_TENSOR_DIM_APPLY_i = 0; TH_TENSOR_DIM_APPLY_i < TH##CTYPE##Tensor_nDimension(TENSOR); TH_TENSOR_DIM_APPLY_i++) \
      {                                                                 \
        if(TH_TENSOR_DIM_APPLY_i == DIMENSION)                          \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH##CTYPE##Tensor_nDimension(TENSOR)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          continue;                                                     \
        }                                                               \
                                                                        \
        TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]++;           \
        TENSOR##_data += TH##CTYPE##Tensor_stride(TENSOR, TH_TENSOR_DIM_APPLY_i); \
                                                                        \
        if(TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] == TH##CTYPE##Tensor_size(TENSOR, TH_TENSOR_DIM_APPLY_i)) \
        {                                                               \
          if(TH_TENSOR_DIM_APPLY_i == TH##CTYPE##Tensor_nDimension(TENSOR)-1) \
          {                                                             \
            TH_TENSOR_DIM_APPLY_hasFinished = 1;                        \
            break;                                                      \
          }                                                             \
          else                                                          \
          {                                                             \
            TENSOR##_data -= TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i]*TH##CTYPE##Tensor_stride(TENSOR, TH_TENSOR_DIM_APPLY_i); \
            TH_TENSOR_DIM_APPLY_counter[TH_TENSOR_DIM_APPLY_i] = 0;     \
          }                                                             \
        }                                                               \
        else                                                            \
          break;                                                        \
      }                                                                 \
    }                                                                   \
    THFree(TH_TENSOR_DIM_APPLY_counter);                                \
  }

#endif
