#include "zozo3.h"

int main()
{
  THTensor<char> *aob = new THTensor<char>(10);
  aob->print();

  THTensor<int> *aoi = new THTensor<int>(10);
  aoi->print();

  return 0;
}
