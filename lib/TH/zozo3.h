#include <stdio.h>
#include <stdlib.h>

template <class T> struct THTensor
{
    long size;
    T *stuff;

  public:

    THTensor(long size);
    void print();
};
