#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THTensorCopy.h"
#else

void THTensor_(copyByte)(THTensor *self, THByteTensor *tensor);
void THTensor_(copyChar)(THTensor *self, THCharTensor *tensor);
void THTensor_(copyShort)(THTensor *self, THShortTensor *tensor);
void THTensor_(copyInt)(THTensor *self, THIntTensor *tensor);
void THTensor_(copyLong)(THTensor *self, THLongTensor *tensor);
void THTensor_(copyFloat)(THTensor *self, THFloatTensor *tensor);
void THTensor_(copyDouble)(THTensor *self, THDoubleTensor *tensor);

#endif
