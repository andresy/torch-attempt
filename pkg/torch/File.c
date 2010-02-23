#include "File.h"

static const void *torch_File_id = NULL;

static int torch_File_binary(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isBinary = 1;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_ascii(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isBinary = 0;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_autoSpacing(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isAutoSpacing = 1;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_noAutoSpacing(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isAutoSpacing = 0;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_quiet(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isQuiet = 1;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_isQuiet(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  lua_pushboolean(L, file->isQuiet);
  return 1;
}

static int torch_File_pedantic(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->isQuiet = 0;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_clearError(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  file->hasError = 0;
  lua_settop(L, 1);
  return 1;
}

static int torch_File_hasError(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  lua_pushboolean(L, file->hasError);
  return 1;
}

static int torch_File_isReadable(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  lua_pushboolean(L, file->isReadable);
  return 1;
}

static int torch_File_isWritable(lua_State *L)
{
  File *file = luaT_checkudata(L, 1, torch_File_id);
  lua_pushboolean(L, file->isWritable);
  return 1;
}

static const struct luaL_Reg torch_File__ [] = {
  {"binary", torch_File_binary},
  {"ascii", torch_File_ascii},
  {"autoSpacing", torch_File_autoSpacing},
  {"noAutoSpacing", torch_File_noAutoSpacing},
  {"quiet", torch_File_quiet},
  {"isQuiet", torch_File_isQuiet},
  {"pedantic", torch_File_pedantic},
  {"hasError", torch_File_hasError},
  {"clearError", torch_File_clearError},
  {"isReadable", torch_File_isReadable},
  {"isWritable", torch_File_isWritable},
  {NULL, NULL}
};

void torch_File_init(lua_State *L)
{
  torch_File_id = luaT_newmetatable(L, "torch.File", NULL, NULL, NULL, NULL);
  luaL_register(L, NULL, torch_File__);
  lua_pop(L, 1);
}
