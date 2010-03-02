#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/lab.c"
#else

static const void* torch_(Tensor_id);
static const void* torch_LongStorage_id;

static int lab_(rand)(lua_State *L)
{
  THLongStorage *dimension = lab_checklongargs(L, 1);
  THTensor *t = THTensor_(new)();
  luaT_pushudata(L, t, torch_(Tensor_id));
  THLab_(rand)(dimension, t);

  lua_getfield(L, LUA_GLOBALSINDEX, "print");
  THLongStorage_retain(dimension);
  luaT_pushudata(L, dimension, torch_LongStorage_id);
  lua_call(L, 1, 0);

  THLongStorage_free(dimension);
  return 1;
}

static const struct luaL_Reg lab_(stuff__) [] = {
  {"rand_", lab_(rand)},
  {NULL, NULL}
};

void lab_(init)(lua_State *L)
{
  torch_(Tensor_id) = luaT_checktypename2id(L, torch_string_(Tensor));
  torch_LongStorage_id = luaT_checktypename2id(L, "torch.LongStorage");
  luaL_register(L, NULL, lab_(stuff__));
}

#endif
