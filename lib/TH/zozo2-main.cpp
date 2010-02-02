#include "zozo2.h"

int main()
{
  THTensor<char> *aob;
  aob = THTensor_new<char>(10);
  THTensor_print(aob);

  THTensor<int> *aoi;
  aoi = THTensor_new<int>(10);
  THTensor_print(aoi);

  return 0;
}
