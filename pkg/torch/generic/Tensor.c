#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/Tensor.c"
#else

static void torch_(Tensor_c_readTensorStorageSize)(lua_State *L, int index, int allowNone, int allowTensor, int allowStorage,
                                                         THStorage **storage_, long *storageOffset_, THLongStorage **size_);

static void torch_(Tensor_c_readSize)(lua_State *L, int index, THLongStorage **size_);

static int torch_(Tensor_size)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));

  if(lua_isnumber(L,2))
  {
    int dim = luaL_checkint(L, 2)-1;
    luaL_argcheck(L, dim >= 0 && dim < THTensor_(nDimension)(tensor), 2, "out of range");
    lua_pushnumber(L, THTensor_(size)(tensor, dim));
  }
  else
    luaT_pushudata(L, THTensor_(newSizeOf)(tensor), torch_LongStorage_id);

  return 1;
}

static int torch_(Tensor_stride)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));

  if(lua_isnumber(L,2))
  {
    int dim = luaL_checkint(L, 2)-1;
    luaL_argcheck(L, dim >= 0 && dim < THTensor_(nDimension)(tensor), 2, "out of range");
    lua_pushnumber(L, THTensor_(stride)(tensor, dim));
  }
  else
    luaT_pushudata(L, THTensor_(newStrideOf)(tensor), torch_LongStorage_id);

  return 1;
}

static int torch_(Tensor_nDimension)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  lua_pushnumber(L, THTensor_(nDimension)(tensor));
  return 1;
}

static int torch_(Tensor_storage)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THStorage_(retain)(THTensor_(storage)(tensor));
  luaT_pushudata(L, THTensor_(storage)(tensor), torch_(Storage_id));
  return 1;
}

static int torch_(Tensor_storageOffset)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  lua_pushnumber(L, THTensor_(storageOffset)(tensor)+1);
  return 1;
}

static int torch_(Tensor_new)(lua_State *L)
{
  THTensor *tensor;
  THStorage *storage = NULL;
  long storageOffset = 0;
  THLongStorage *size = NULL;

  torch_(Tensor_c_readTensorStorageSize)(L, 1, 1, 1, 1,
                                               &storage, &storageOffset, &size);

  tensor = THTensor_(newWithStorage)(storage, storageOffset, size);
  THLongStorage_free(size);

  luaT_pushudata(L, tensor, torch_(Tensor_id));
  return 1;
}

/* Resize */
static int torch_(Tensor_resizeAs)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  THTensor_(resizeAs)(tensor, src);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_resize)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THLongStorage *size;

  torch_(Tensor_c_readSize)(L, 2, &size);

  THTensor_(resize)(tensor, size);
  THLongStorage_free(size);

  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_narrow)(lua_State *L)
{
  THTensor *src = luaT_checkudata(L, 1, torch_(Tensor_id));
  long firstIndex = luaL_checklong(L, 2)-1;
  long size = luaL_checklong(L, 3);

  THTensor *tensor = THTensor_(newWithTensorNarrow)(src, firstIndex, size);
  luaT_pushudata(L, tensor, torch_(Tensor_id));
  return 1;
}

static int torch_(Tensor_select)(lua_State *L)
{
  THTensor *src = luaT_checkudata(L, 1, torch_(Tensor_id));
  long sliceIndex = luaL_checklong(L, 2)-1;
  THTensor *tensor = THTensor_(newWithTensorSelect)(src, sliceIndex);
  luaT_pushudata(L, tensor, torch_(Tensor_id));
  return 1;
}

static int torch_(Tensor_nElement)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  lua_pushnumber(L, THTensor_(nElement)(tensor));
  return 1;
}

static int torch_(Tensor_copy)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  void *src;
  if( (src = luaT_toudata(L, 2, torch_(Tensor_id))) )
    THTensor_(copy)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_ByteTensor_id)) )
    THTensor_(copyByte)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_CharTensor_id)) )
    THTensor_(copyChar)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_ShortTensor_id)) )
    THTensor_(copyShort)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_IntTensor_id)) )
    THTensor_(copyInt)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_LongTensor_id)) )
    THTensor_(copyLong)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_FloatTensor_id)) )
    THTensor_(copyFloat)(tensor, src);
  else if( (src = luaT_toudata(L, 2, torch_DoubleTensor_id)) )
    THTensor_(copyDouble)(tensor, src);
  else
    luaL_typerror(L, 2, "torch.*Tensor");
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor___newindex__)(lua_State *L)
{
  if(lua_isnumber(L, 2))
  {
    THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
    long index = luaL_checklong(L,2)-1;
    real value = (real)luaL_checknumber(L,3);

    luaL_argcheck(L, THTensor_(nDimension)(tensor) == 1, 1, "must be a one dimensional tensor");
    THTensor_(set1d)(tensor, index, value);
    lua_pushboolean(L, 1);
  }
  else
    lua_pushboolean(L, 0);

  return 1;
}

static int torch_(Tensor___index__)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));

  if(lua_isnumber(L, 2))
  {
    long index = luaL_checklong(L,2)-1;    
    luaL_argcheck(L, THTensor_(nDimension)(tensor) > 0, 1, "empty tensor");

    if(THTensor_(nDimension)(tensor) == 1)
      lua_pushnumber(L, THTensor_(get1d)(tensor, index));
    else
    {
      THTensor *tensor_ = THTensor_(newWithTensorSelect)(tensor, index);
      luaT_pushudata(L, tensor_, torch_(Tensor_id));
    }
    lua_pushboolean(L, 1);
    return 2;
  }
  else
  {
    lua_pushboolean(L, 0);
    return 1;
  }
}

static int torch_(Tensor_free)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor_(free)(tensor);
  return 0;
}

/* helpful functions */
static void torch_(Tensor_c_readSize)(lua_State *L, int index, THLongStorage **size_)
{
  THLongStorage *size = NULL;
  long i;

  if( (size = luaT_toudata(L, index, torch_LongStorage_id)) )
  {
    THLongStorage_retain(size);
    *size_ = size;
  }
  else
  {
    size = THLongStorage_newWithSize(4);
    for(i = 0; i < 4; i++)
    {
      if(lua_isnone(L, index+i))
        THLongStorage_set(size, i, 0);
      else
      {
        if(lua_isnumber(L, index+i))
          THLongStorage_set(size, i, lua_tonumber(L, index+i));
        else
        {
          THLongStorage_free(size);
          luaL_error(L, "invalid argument %d: number expected", index+i);
        }
      }
    }
    *size_ = size;
  }
}

static void torch_(Tensor_c_readTensorStorageSize)(lua_State *L, int index, int allowNone, int allowTensor, int allowStorage,
                                                         THStorage **storage_, long *storageOffset_, THLongStorage **size_)
{
  static char errMsg[64];
  THTensor *src = NULL;
  THStorage *storage = NULL;

  int arg1Type = lua_type(L, index);

  if( allowNone && (arg1Type == LUA_TNONE) )
  {
    *storage_ = NULL;
    *storageOffset_ = 0;
    *size_ = THLongStorage_new();
    return;
  }
  else if( allowTensor && (arg1Type == LUA_TUSERDATA) && (src = luaT_toudata(L, index, torch_(Tensor_id))) )
  {
    *storage_ = THTensor_(storage)(src);
    *storageOffset_ = THTensor_(storageOffset)(src);
    *size_ = THTensor_(newSizeOf)(src);
    return;
  }
  else if( allowStorage && (arg1Type == LUA_TUSERDATA) && (storage = luaT_toudata(L, index, torch_(Storage_id))) )
  {
    *storage_ = storage;
    if(lua_isnone(L, index+1))
    {
      *storageOffset_ = 0;
      *size_ = THLongStorage_newWithSize(1);
      THLongStorage_set(*size_, 1, THStorage_(size)(storage));
    }
    else
    {
      *storageOffset_ = luaL_checklong(L, index+1)-1;
      torch_(Tensor_c_readSize)(L, index+2, size_);
    }
    return;
  }
  else if( (arg1Type == LUA_TNUMBER) || (luaT_toudata(L, index, torch_LongStorage_id)) )
  {
    *storage_ = NULL;
    *storageOffset_ = 0;
    torch_(Tensor_c_readSize)(L, index, size_);
    return;
  }
  sprintf(errMsg, "expecting number%s%s", (allowTensor ? " or Tensor" : ""), (allowStorage ? " or Storage" : ""));
  luaL_argcheck(L, 0, index, errMsg);
}

static int torch_(Tensor_apply)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));

  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_settop(L, 2);


  TH_TENSOR_APPLY(tensor,
                  lua_pushvalue(L, 2);
                  lua_pushnumber(L, tensor_data[i]);
                  lua_call(L, 1, 1);
                  if(lua_isnumber(L, 3))
                  {
                    tensor_data[i] = (real)lua_tonumber(L, 3);
                    lua_pop(L, 1);
                  }
                  else if(lua_isnil(L, 3))
                    lua_pop(L, 1);
                  else
                    luaL_error(L, "given function should return a number or nil"););

  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_map)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  luaL_checktype(L, 3, LUA_TFUNCTION);
  lua_settop(L, 3);

  TH_TENSOR_APPLY2(tensor, src,
                  lua_pushvalue(L, 3);
                   lua_pushnumber(L, tensor_data[i]);
                  lua_pushnumber(L, src_data[i]);
                  lua_call(L, 2, 1);
                  if(lua_isnumber(L, 4))
                  {
                    tensor_data[i] = (real)lua_tonumber(L, 4);
                    lua_pop(L, 1);
                  }
                  else if(lua_isnil(L, 4))
                    lua_pop(L, 1);
                  else
                    luaL_error(L, "given function should return a number or nil"););

  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_map2)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src1 = luaT_checkudata(L, 2, torch_(Tensor_id));
  THTensor *src2 = luaT_checkudata(L, 3, torch_(Tensor_id));
  luaL_checktype(L, 4, LUA_TFUNCTION);
  lua_settop(L, 4);

  TH_TENSOR_APPLY3(tensor, src1, src2,
                  lua_pushvalue(L, 4);
                  lua_pushnumber(L, tensor_data[i]);
                  lua_pushnumber(L, src1_data[i]);
                  lua_pushnumber(L, src2_data[i]);
                  lua_call(L, 3, 1);
                  if(lua_isnumber(L, 5))
                  {
                    tensor_data[i] = (real)lua_tonumber(L, 5);
                    lua_pop(L, 1);
                  }
                  else if(lua_isnil(L, 5))
                    lua_pop(L, 1);
                  else
                    luaL_error(L, "given function should return a number or nothing"););

  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_factory)(lua_State *L)
{
  THTensor *tensor = THTensor_(new)();
  luaT_pushudata(L, tensor, torch_(Tensor_id));
  return 1;
}

static int torch_(Tensor_write)(lua_State *L)
{  
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));

  lua_getfield(L, 2, "writeInt");
  lua_pushvalue(L, 2);
  lua_pushnumber(L, THTensor_(nDimension)(tensor));
  lua_call(L, 2, 0);

  lua_getfield(L, 2, "writeLong");
  lua_pushvalue(L, 2);
  luaL_callmeta(L, 1, "size");
  lua_call(L, 2, 0);

  lua_getfield(L, 2, "writeLong");
  lua_pushvalue(L, 2);
  lua_pushnumber(L, THTensor_(storageOffset)(tensor)+1); /* to respect Lua convention */
  lua_call(L, 2, 0);

  lua_getfield(L, 2, "writeObject");
  lua_pushvalue(L, 2);
  luaL_callmeta(L, 1, "storage");
  lua_call(L, 2, 0);

  return 0;
}

static int torch_(Tensor_read)(lua_State *L)
{
  THTensor *self = luaT_checkudata(L, 1, torch_(Tensor_id));
  THStorage *storage;
  THLongStorage *size;
  int nDimension;
  long storageOffset;

  lua_getfield(L, 2, "readInt");
  lua_pushvalue(L, 2);
  lua_call(L, 1, 1);
  nDimension = lua_tonumber(L, -1);

  lua_getfield(L, 2, "readLong");
  lua_pushvalue(L, 2);
  lua_pushnumber(L, nDimension);
  lua_call(L, 2, 1);
  size = luaT_toudata(L, -1, torch_LongStorage_id);

  lua_getfield(L, 2, "readLong");
  lua_pushvalue(L, 2);
  lua_call(L, 1, 1);
  storageOffset = lua_tonumber(L, -1)-1;

  lua_getfield(L, 2, "readObject");
  lua_pushvalue(L, 2);
  lua_call(L, 1, 1);
  storage = luaT_toudata(L, -1, torch_(Storage_id));

  THTensor_(init)(self, storage, storageOffset, size);

  return 0;
}

static const struct luaL_Reg torch_(Tensor__) [] = {
  {"size", torch_(Tensor_size)},
  {"__len__", torch_(Tensor_size)},
  {"stride", torch_(Tensor_stride)},
  {"dim", torch_(Tensor_nDimension)},
  {"nDimension", torch_(Tensor_nDimension)},
  {"storage", torch_(Tensor_storage)},
  {"storageOffset", torch_(Tensor_storageOffset)},
  {"resizeAs", torch_(Tensor_resizeAs)},
  {"resize", torch_(Tensor_resize)},
  {"narrow", torch_(Tensor_narrow)},
  {"select", torch_(Tensor_select)},
  {"nElement", torch_(Tensor_nElement)},
  {"copy", torch_(Tensor_copy)},
  {"apply", torch_(Tensor_apply)},
  {"map", torch_(Tensor_map)},
  {"map2", torch_(Tensor_map2)},
  {"read", torch_(Tensor_read)},
  {"write", torch_(Tensor_write)},
  {"__index__", torch_(Tensor___index__)},
  {"__newindex__", torch_(Tensor___newindex__)},
  {NULL, NULL}
};

void torch_(Tensor_init)(lua_State *L)
{
  torch_ByteStorage_id = luaT_checktypename2id(L, "torch.ByteStorage");
  torch_CharStorage_id = luaT_checktypename2id(L, "torch.CharStorage");
  torch_ShortStorage_id = luaT_checktypename2id(L, "torch.ShortStorage");
  torch_IntStorage_id = luaT_checktypename2id(L, "torch.IntStorage");
  torch_LongStorage_id = luaT_checktypename2id(L, "torch.LongStorage");
  torch_FloatStorage_id = luaT_checktypename2id(L, "torch.FloatStorage");
  torch_DoubleStorage_id = luaT_checktypename2id(L, "torch.DoubleStorage");

  torch_(Tensor_id) = luaT_newmetatable(L, torch_string_(Tensor), NULL,
                                 torch_(Tensor_new), torch_(Tensor_free), torch_(Tensor_factory));

  luaL_register(L, NULL, torch_(Tensor__));
  lua_pop(L, 1);
}

#endif
