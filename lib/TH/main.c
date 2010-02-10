#include "THStorages.h"

int main()
{
  THDoubleStorage *storaged = THDoubleStorageAPI.allocWithSize(10);
  THFloatStorage *storagef = THFloatStorageAPI.allocWithSize(10);

  THDoubleStorageAPI.resize(storaged, 100);
  THDoubleStorageAPI.free(storaged);

  THFloatStorageAPI.resize(storagef, 100);
  THFloatStorageAPI.free(storagef);

  return 0;
}
