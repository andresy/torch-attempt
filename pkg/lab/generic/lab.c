#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/lab.c"
#else

static void* torch_(Tensor_id);

static void lab_(rand)(lua_State *L)
{
  THLongStorage *dimension = lab_checklongargs(L, 1);
  THTensor *t = THTensor_(new)();
  luaT_pushudata(L, t, torch_(Tensor_id));
  THLab_(rand)(dimension, t);
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
  luaL_register(L, NULL, lab_(stuff__));
}

#endif
