#ifndef TORCH_FILE_INC
#define TORCH_FILE_INC

#include "general.h"

typedef struct File__
{
    int isQuiet;
    int isReadable;
    int isWritable;
    int isBinary;
    int isAutoSpacing;
    int hasError;
} File;

void torch_File_init(lua_State *L);

#endif
