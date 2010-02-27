#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/TensorMath.c"
#else

#define TENSOR_IMPLEMENT_BASIC_WRAPPER(FUNC)                      \
  static int torch_(Tensor_##FUNC)(lua_State *L)                  \
  {                                                               \
    THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));  \
    THTensor_(FUNC)(tensor);                                      \
    lua_settop(L, 1);                                             \
    return 1;                                                     \
  }

#define TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(FUNC)                 \
  static int torch_(Tensor_##FUNC)(lua_State *L)                  \
  {                                                               \
    THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));  \
    lua_pushnumber(L, THTensor_(FUNC)(tensor));                   \
    return 1;                                                     \
}

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)

TENSOR_IMPLEMENT_BASIC_WRAPPER(log)
TENSOR_IMPLEMENT_BASIC_WRAPPER(log1p)
TENSOR_IMPLEMENT_BASIC_WRAPPER(exp)
TENSOR_IMPLEMENT_BASIC_WRAPPER(cos)
TENSOR_IMPLEMENT_BASIC_WRAPPER(acos)
TENSOR_IMPLEMENT_BASIC_WRAPPER(cosh)
TENSOR_IMPLEMENT_BASIC_WRAPPER(sin)
TENSOR_IMPLEMENT_BASIC_WRAPPER(asin)
TENSOR_IMPLEMENT_BASIC_WRAPPER(sinh)
TENSOR_IMPLEMENT_BASIC_WRAPPER(tan)
TENSOR_IMPLEMENT_BASIC_WRAPPER(atan)
TENSOR_IMPLEMENT_BASIC_WRAPPER(tanh)
TENSOR_IMPLEMENT_BASIC_WRAPPER(sqrt)
TENSOR_IMPLEMENT_BASIC_WRAPPER(ceil)
TENSOR_IMPLEMENT_BASIC_WRAPPER(floor)
TENSOR_IMPLEMENT_BASIC_WRAPPER(abs)

static int torch_(Tensor_pow)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_checknumber(L, 2);
  THTensor_(pow)(tensor, value);
  lua_settop(L, 1);
  return 1;
}

TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(mean)
TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(var)
TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(std)

static int torch_(Tensor_norm)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_optnumber(L, 2, 2);
  lua_pushnumber(L, THTensor_(norm)(tensor, value));
  return 1;
}

static int torch_(Tensor_dist)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  double value = luaL_optnumber(L, 3, 2);
  lua_pushnumber(L, THTensor_(dist)(tensor, src, value));
  return 1;
}

#endif

TENSOR_IMPLEMENT_BASIC_WRAPPER(zero)

static int torch_(Tensor_add)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = NULL;
  int n = lua_gettop(L);

  if( (n == 2) && lua_isnumber(L, 2) )
  {
    double value = luaL_checknumber(L, 2);
    THTensor_(add)(tensor, value);
  }
  else if( (n == 2) && (src = luaT_toudata(L, 2, torch_(Tensor_id))) )
    THTensor_(addTensor)(tensor, 1, src);
  else if( (n == 3) && lua_isnumber(L, 2) && (src = luaT_toudata(L, 3, torch_(Tensor_id))) )
  {
    double value = luaL_checknumber(L, 2);
    THTensor_(addTensor)(tensor, value, src);
  }
  else
    luaL_error(L, "bad arguments: number, torch.Tensor, or number and torch.Tensor expected");

  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_mul)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_checknumber(L, 2);
  THTensor_(mul)(tensor, value);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_cmul)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  THTensor_(cmul)(tensor, src);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_addcmul)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_checknumber(L, 2);
  THTensor *src1 = luaT_checkudata(L, 3, torch_(Tensor_id));
  THTensor *src2 = luaT_checkudata(L, 4, torch_(Tensor_id));
  THTensor_(addcmul)(tensor, value, src1, src2);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_div)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_checknumber(L, 2);
  THTensor_(div)(tensor, value);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_cdiv)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  THTensor_(cdiv)(tensor, src);
  lua_settop(L, 1);
  return 1;
}

static int torch_(Tensor_addcdiv)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double value = luaL_checknumber(L, 2);
  THTensor *src1 = luaT_checkudata(L, 3, torch_(Tensor_id));
  THTensor *src2 = luaT_checkudata(L, 4, torch_(Tensor_id));
  THTensor_(addcdiv)(tensor, value, src1, src2);
  lua_settop(L, 1);
  return 1;
}

/* statistics */


TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(min)
TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(max)
TENSOR_IMPLEMENT_BASIC_WRAPPER_STAT(sum)

/* basic linear algebra */

static int torch_(Tensor_dot)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *src = luaT_checkudata(L, 2, torch_(Tensor_id));
  lua_pushnumber(L, THTensor_(dot)(tensor, src));
  return 1;
}

static int torch_(Tensor_addmv)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double alpha = luaL_checknumber(L, 2);
  int transpose1 = luaT_checkboolean(L, 3);
  THTensor *src1 = luaT_checkudata(L, 4, torch_(Tensor_id));
  THTensor *src2 = luaT_checkudata(L, 5, torch_(Tensor_id));

  THTensor_(addmv)(tensor, alpha, transpose1, src1, src2);
  lua_settop(L, 1);

  return 1;
}

static int torch_(Tensor_addr)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double alpha = luaL_checknumber(L, 2);
  THTensor *src1 = luaT_checkudata(L, 3, torch_(Tensor_id));
  THTensor *src2 = luaT_checkudata(L, 4, torch_(Tensor_id));

  THTensor_(addr)(tensor, alpha, src1, src2);
  lua_settop(L, 1);

  return 1;
}

static int torch_(Tensor_addmm)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  double alpha = luaL_checknumber(L, 2);
  int transpose1 = luaT_checkboolean(L, 3);
  THTensor *src1 = luaT_checkudata(L, 4, torch_(Tensor_id));
  int transpose2 = luaT_checkboolean(L, 5);
  THTensor *src2 = luaT_checkudata(L, 6, torch_(Tensor_id));

  THTensor_(addmm)(tensor, alpha, transpose1, src1, transpose2, src2);
  lua_settop(L, 1);

  return 1;
}

static int torch_(Tensor___add__)(lua_State *L)
{
  THTensor *tensor1 = luaT_toudata(L, 1, torch_(Tensor_id));
  THTensor *tensor2 = luaT_toudata(L, 2, torch_(Tensor_id));
  THTensor *r;

  if(!tensor1 && !tensor2)
    luaL_error(L, "expecting two Tensors or one Tensor and one number");
  else
  {
    r = THTensor_(new)();
    luaT_pushudata(L, r, torch_(Tensor_id));
    
    if(!tensor1 && tensor2)
    {
      THTensor_(resizeAs)(r, tensor2);
      THTensor_(copy)(r, tensor2);
      THTensor_(add)(r, luaL_checknumber(L, 1));
    }
    else if(tensor1 && !tensor2)
    {
      THTensor_(resizeAs)(r, tensor1);
      THTensor_(copy)(r, tensor1);
      THTensor_(add)(r, luaL_checknumber(L, 2));
    }
    else
    {
      THTensor_(resizeAs)(r, tensor1);
      THTensor_(copy)(r, tensor1);
      THTensor_(addTensor)(r, 1, tensor2);
    }
  }
  return 1;
}

static int torch_(Tensor___sub__)(lua_State *L)
{
  THTensor *tensor1 = luaT_toudata(L, 1, torch_(Tensor_id));
  THTensor *tensor2 = luaT_toudata(L, 2, torch_(Tensor_id));
  THTensor *r;

  if(!tensor1 && !tensor2)
    luaL_error(L, "expecting two Tensors or one Tensor and one number");
  else
  {
    r = THTensor_(new)();
    luaT_pushudata(L, r, torch_(Tensor_id));
    
    if(!tensor1 && tensor2)
    {
      THTensor_(resizeAs)(r, tensor2);
      THTensor_(fill)(r, luaL_checknumber(L, 1));
      THTensor_(addTensor)(r, -1, tensor2);
    }
    else if(tensor1 && !tensor2)
    {
      THTensor_(resizeAs)(r, tensor1);
      THTensor_(copy)(r, tensor1);
      THTensor_(add)(r, -luaL_checknumber(L, 2));
    }
    else
    {
      THTensor_(resizeAs)(r, tensor1);
      THTensor_(copy)(r, tensor1);
      THTensor_(addTensor)(r, -1, tensor2);
    }
  }
  return 1;
}

static int torch_(Tensor___unm__)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *r;

  r = THTensor_(new)();
  luaT_pushudata(L, r, torch_(Tensor_id));
  THTensor_(resizeAs)(r, tensor);
  THTensor_(copy)(r, tensor);
  THTensor_(mul)(r, -1);

  return 1;
}

static int torch_(Tensor___mul__)(lua_State *L)
{
  THTensor *tensor1 = luaT_toudata(L, 1, torch_(Tensor_id));
  THTensor *tensor2 = luaT_toudata(L, 2, torch_(Tensor_id));
  THTensor *r;

  if(!tensor1 && !tensor2)
    luaL_error(L, "expecting two Tensors or one Tensor and one number");
  else
  {
    r = THTensor_(new)();
    luaT_pushudata(L, r, torch_(Tensor_id));
    
    if(!tensor1 && tensor2)
    {
      THTensor_(resizeAs)(r, tensor2);
      THTensor_(copy)(r, tensor2);
      THTensor_(mul)(r, luaL_checknumber(L, 1));
    }
    else if(tensor1 && !tensor2)
    {
      THTensor_(resizeAs)(r, tensor1);
      THTensor_(copy)(r, tensor1);
      THTensor_(mul)(r, luaL_checknumber(L, 2));
    }
    else
    {
      int dimt = THTensor_(nDimension)(tensor1);
      int dims = THTensor_(nDimension)(tensor2);
      
      if(dimt == 1 && dims == 1)
        lua_pushnumber(L, THTensor_(dot)(tensor1, tensor2)); /* ok, we wasted r, but who cares */
      else if(dimt == 2 && dims == 1)
      {
        THTensor_(resize1d)(r, THTensor_(size)(tensor1, 0));
        THTensor_(zero)(r);
        THTensor_(addmv)(r, 1, 0, tensor1, tensor2);
      }
      else if(dimt == 2 && dims == 2)
      {
        THTensor_(resize2d)(r, THTensor_(size)(tensor1, 0), THTensor_(size)(tensor2, 1));
        THTensor_(zero)(r);
        THTensor_(addmm)(r, 1, 0, tensor1, 0, tensor2);
      }
      else
        luaL_error(L, "multiplication between %dD and %dD tensors not yet supported", THTensor_(nDimension)(tensor1), THTensor_(nDimension)(tensor2)); 
    }
  }
  return 1;
}

static int torch_(Tensor___div__)(lua_State *L)
{
  THTensor *tensor = luaT_checkudata(L, 1, torch_(Tensor_id));
  THTensor *r;

  luaL_argcheck(L, lua_isnumber(L,2), 2, "number expected");

  r = THTensor_(new)();
  luaT_pushudata(L, r, torch_(Tensor_id));

  THTensor_(resizeAs)(r, tensor);
  THTensor_(copy)(r, tensor);
  THTensor_(mul)(r, 1/lua_tonumber(L, 2));

  return 1;
}

static const struct luaL_Reg torch_(Tensor__) [] = {
#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)
  {"log", torch_(Tensor_log)},
  {"log1p", torch_(Tensor_log1p)},
  {"exp", torch_(Tensor_exp)},
  {"cos", torch_(Tensor_cos)},
  {"acos", torch_(Tensor_acos)},
  {"cosh", torch_(Tensor_cosh)},
  {"sin", torch_(Tensor_sin)},
  {"asin", torch_(Tensor_asin)},
  {"sinh", torch_(Tensor_sinh)},
  {"tan", torch_(Tensor_tan)},
  {"atan", torch_(Tensor_atan)},
  {"tanh", torch_(Tensor_tanh)},
  {"sqrt", torch_(Tensor_sqrt)},
  {"ceil", torch_(Tensor_ceil)},
  {"floor", torch_(Tensor_floor)},
  {"abs", torch_(Tensor_abs)},
  {"pow", torch_(Tensor_pow)},
  {"mean", torch_(Tensor_mean)},
  {"var", torch_(Tensor_var)},
  {"std", torch_(Tensor_std)},
  {"norm", torch_(Tensor_norm)},
  {"dist", torch_(Tensor_dist)},
#endif
  {"zero", torch_(Tensor_zero)},
  {"add", torch_(Tensor_add)},
  {"mul", torch_(Tensor_mul)},
  {"cmul", torch_(Tensor_cmul)},
  {"addcmul", torch_(Tensor_addcmul)},
  {"div", torch_(Tensor_div)},
  {"cdiv", torch_(Tensor_cdiv)},
  {"addcdiv", torch_(Tensor_addcdiv)},
  {"min", torch_(Tensor_min)},
  {"max", torch_(Tensor_max)},
  {"sum", torch_(Tensor_sum)},
  {"dot", torch_(Tensor_dot)},
  {"addmv", torch_(Tensor_addmv)},
  {"addr", torch_(Tensor_addr)},
  {"addmm", torch_(Tensor_addmm)},
  {"__add__", torch_(Tensor___add__)},
  {"__sub__", torch_(Tensor___sub__)},
  {"__unm__", torch_(Tensor___unm__)},
  {"__mul__", torch_(Tensor___mul__)},
  {"__div__", torch_(Tensor___div__)},
  {NULL, NULL}
};

void torch_(TensorMath_init)(lua_State *L)
{
  torch_(Tensor_id) = luaT_checktypename2id(L, torch_string_(Tensor));

  luaT_pushmetaclass(L, torch_(Tensor_id));
  luaL_register(L, NULL, torch_(Tensor__));
  lua_pop(L, 1);
}

#endif
