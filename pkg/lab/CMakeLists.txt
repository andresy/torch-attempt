SET(src lab.c utils.c init.c)
SET(luasrc init.lua)
  
ADD_TORCH_PACKAGE(lab "${src}" "${luasrc}" "Lab")

TARGET_LINK_LIBRARIES(lab luaT TH)
