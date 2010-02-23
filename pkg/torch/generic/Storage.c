#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/Storage.c"
#else

static int torch_(Storage_new)(lua_State *L)
{
  THStorage *storage;
  if(lua_type(L, 1) == LUA_TSTRING)
  {
    const char *fileName = luaL_checkstring(L, 1);
    int isShared = luaT_optboolean(L, 2, 0);
    storage = THStorage_(newWithMapping)(fileName, isShared);
  }
  else
  {
    long size = luaL_optlong(L, 1, 0);
    storage = THStorage_(newWithSize)(size);
  }
  luaT_pushudata(L, storage, torch_(Storage_id));
  return 1;
}

static int torch_(Storage_free)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  THStorage_(free)(storage);
  return 0;
}

static int torch_(Storage_resize)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  long size = luaL_checklong(L, 2);
  THStorage_(resize)(storage, size);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Storage_copy)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  void *src;
  if( (src = luaT_toudata(L, 2, torch_(Storage_id))) )
    THStorage_(copy)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_ByteStorage_id)) )
    THStorage_(copyByte)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_CharStorage_id)) )
    THStorage_(copyChar)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_ShortStorage_id)) )
    THStorage_(copyShort)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_IntStorage_id)) )
    THStorage_(copyInt)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_LongStorage_id)) )
    THStorage_(copyLong)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_FloatStorage_id)) )
    THStorage_(copyFloat)(storage, src);
  else if( (src = luaT_toudata(L, 2, torch_DoubleStorage_id)) )
    THStorage_(copyDouble)(storage, src);
  else
    luaL_typerror(L, 2, "torch.*Storage");
  lua_settop(L, 1);
  return 1;
}

static int torch_(Storage_fill)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  double value = luaL_checknumber(L, 2);
  THStorage_(fill)(storage, (real)value);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Storage___len__)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  lua_pushnumber(L, THStorage_(size)(storage));
  return 1;
}

static int torch_(Storage___newindex__)(lua_State *L)
{
  if(lua_isnumber(L, 2))
  {
    THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
    long index = luaL_checklong(L, 2) - 1;
    double number = luaL_checknumber(L, 3);
    THStorage_(set)(storage, index, number);
    lua_pushboolean(L, 1);
  }
  else
    lua_pushboolean(L, 0);

  return 1;
}

static int torch_(Storage___index__)(lua_State *L)
{
  if(lua_isnumber(L, 2))
  {
    THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
    long index = luaL_checklong(L, 2) - 1;
    THStorage_(get)(storage, index);
    return 2;
  }
  else
  {
    lua_pushboolean(L, 0);
    return 1;
  }
}

#if defined(TH_REAL_IS_CHAR) || defined(TH_REAL_IS_BYTE)
static int torch_(Storage_string)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  if(lua_isstring(L, -1))
  {
    size_t len = 0;
    const char *str = lua_tolstring(L, -1, &len);
    THStorage_(resize)(storage, len);
    memmove(THStorage_(data)(storage), str, len);
    lua_settop(L, 1);
  }
  else
    lua_pushlstring(L, (char*)THStorage_(data)(storage), THStorage_(size)(storage));

  return 1; /* either storage or string */
}
#endif

static int torch_(Storage_factory)(lua_State *L)
{
  THStorage *storage = THStorage_(new)();
  luaT_pushudata(L, storage, torch_(Storage_id));
  return 1;
}

static int torch_(Storage_write)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  long size = THStorage_(size)(storage);

  lua_getfield(L, 2, "writeLong");
  lua_pushvalue(L, 2);
  lua_pushnumber(L, size);
  lua_call(L, 2, 0);

  lua_getfield(L, 2, TH_CONCAT_STRING_2(write, Real));
  lua_pushvalue(L, 2);
  lua_pushvalue(L, 1);
  lua_call(L, 2, 0);

  return 0;
}

static int torch_(Storage_read)(lua_State *L)
{
  THStorage *storage = luaT_checkudata(L, 1, torch_(Storage_id));
  long size;

  lua_getfield(L, 2, "readLong");
  lua_pushvalue(L, 2);
  lua_call(L, 1, 1);
  size = (long)lua_tonumber(L, -1);
  lua_pop(L, 1);

  THStorage_(resize)(storage, size);
  lua_getfield(L, 2, TH_CONCAT_STRING_2(read, Real));
  lua_pushvalue(L, 2);
  lua_pushvalue(L, 1);
  lua_call(L, 2, 0);

  return 0;
}

static const struct luaL_Reg torch_(Storage__) [] = {
  {"size", torch_(Storage___len__)},
  {"__len__", torch_(Storage___len__)},
  {"__newindex__", torch_(Storage___newindex__)},
  {"__index__", torch_(Storage___index__)},
  {"resize", torch_(Storage_resize)},
  {"fill", torch_(Storage_fill)},
  {"copy", torch_(Storage_copy)},
  {"write", torch_(Storage_write)},
  {"read", torch_(Storage_read)},
#if defined(TH_REAL_IS_CHAR) || defined(TH_REAL_IS_BYTE)
  {"string", torch_(Storage_string)},
#endif
  {NULL, NULL}
};

void torch_(Storage_init)(lua_State *L)
{
  torch_(Storage_id) = luaT_newmetatable(L, torch_string_(Storage), NULL,
                                  torch_(Storage_new), torch_(Storage_free), torch_(Storage_factory));

  luaL_register(L, NULL, torch_(Storage__));
  lua_pop(L, 1);
}

#endif
