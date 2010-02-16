#include "THStorage.h"

int main()
{
  THDoubleStorage *storaged = THDoubleStorage_allocWithSize(10);
  THFloatStorage *storagef = THFloatStorage_allocWithSize(10);

  THDoubleStorage_resize(storaged, 100);
  THDoubleStorage_free(storaged);

  THFloatStorage_resize(storagef, 100);
  THFloatStorage_free(storagef);

  return 0;
}
