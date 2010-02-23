#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THStorageCopy.h"
#else

void THStorage_(copyByte)(THStorage *self, THByteStorage *storage);
void THStorage_(copyChar)(THStorage *self, THCharStorage *storage);
void THStorage_(copyShort)(THStorage *self, THShortStorage *storage);
void THStorage_(copyInt)(THStorage *self, THIntStorage *storage);
void THStorage_(copyLong)(THStorage *self, THLongStorage *storage);
void THStorage_(copyFloat)(THStorage *self, THFloatStorage *storage);
void THStorage_(copyDouble)(THStorage *self, THDoubleStorage *storage);

#endif
