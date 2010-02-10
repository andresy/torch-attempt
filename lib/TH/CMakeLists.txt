SET(src
  THGeneral.c
  THByteStorage.c THCharTensor.c THShortStorage.c THIntStorage.c THLongStorage.c THFloatStorage.c THDoubleStorage.c
  THByteTensor.c THCharStorage.c THShortTensor.c THIntTensor.c THLongTensor.c THFloatTensor.c THDoubleTensor.c)

ADD_LIBRARY(TH ${src})

ADD_EXECUTABLE(main main.c)
TARGET_LINK_LIBRARIES(main TH)
