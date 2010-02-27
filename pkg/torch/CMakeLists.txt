SET(src File.c DiskFile.c MemoryFile.c PipeFile.c Timer.c Storage.c Tensor.c TensorMath.c utils.c init.c)
SET(luasrc init.lua File.lua Tensor.lua)
  
ADD_TORCH_PACKAGE(torch "${src}" "${luasrc}" "Basics")

TARGET_LINK_LIBRARIES(torch luaT TH)
