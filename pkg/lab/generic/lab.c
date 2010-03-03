#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/lab.c"
#else

static int lab_(max_)(lua_State *L)
{
  THTensor *values_ = luaT_checkudata(L, 1, torch_(Tensor_id));
  THLongTensor *indices_ = luaT_checkudata(L, 2, torch_LongTensor_id);
  THTensor *t = luaT_checkudata(L, 3, torch_(Tensor_id));
  int dimension = (int)(luaL_optnumber(L, 4, 1))-1;

  THLab_(max)(t, dimension, values_, indices_);
  THLongTensor_add(indices_, 1);

  lua_settop(L, 2);  
  return 2;
}

static int lab_(max)(lua_State *L)
{
  luaT_pushudata(L, THTensor_(new)(), torch_(Tensor_id));
  lua_insert(L, 1);
  luaT_pushudata(L, THLongTensor_new(), torch_LongTensor_id);
  lua_insert(L, 2);
  return lab_(max_)(L);
}

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)

static int lab_(rand_)(lua_State *L)
{
  THTensor *r_ = luaT_checkudata(L, 1, torch_(Tensor_id));
  THLongStorage *dimension = lab_checklongargs(L, 2);

  THLab_(rand)(dimension, r_);

  lua_settop(L, 1);  
  return 1;
}

static int lab_(rand)(lua_State *L)
{
  luaT_pushudata(L, THTensor_(new)(), torch_(Tensor_id));
  lua_insert(L, 1);
  return lab_(rand_)(L);
}

#endif

static const struct luaL_Reg lab_(stuff__) [] = {
  {"max_", lab_(max_)},
  {"max", lab_(max)},
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)
  {"rand_", lab_(rand_)},
  {"rand", lab_(rand)},
#endif
  {NULL, NULL}
};

void lab_(init)(lua_State *L)
{
  torch_(Tensor_id) = luaT_checktypename2id(L, torch_string_(Tensor));
  torch_LongStorage_id = luaT_checktypename2id(L, "torch.LongStorage");
  luaT_registeratid(L, lab_(stuff__), torch_(Tensor_id));
}

#endif
