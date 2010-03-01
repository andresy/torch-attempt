require "torch"

function fillup(x)
   local i = 0
   x:apply(function()
              i = i + 1
              return i
           end)
end

function myselect(n, ...)
   return arg[n]
end

function checksize(t, ndim, offset, ...)
   assert(t:nDimension() == ndim)
   assert(t:storageOffset() == offset)
-- LOUCHE   print('ARG', arg)
   local nElement = 1
   local z = t:size()
   for i = 1,ndim do
--      print(i, 'XXX', t:size(i), myselect(i, ...), arg[i])
      assert(t:size(i) == myselect(i, ...))
      assert(t:size(i) == z[i])
      nElement = nElement * t:size(i)
   end
   assert(nElement == t:nElement())

   local stride = 1
   z = t:stride()
   for i = ndim,1,-1 do
      assert(z[i] == stride)
      assert(z[i] == t:stride(i))
      stride = stride * t:size(i)
   end
end

torch.Tensor = torch.FloatTensor

print('**** basics ****')

t = torch.Tensor(2,3,4,5)
local i = 0
t:apply(function()
           i = i +1
           return i
        end)

i = 0
t:apply(function(z)
           i = i + 1
           assert(z == i)
        end)

i = 0
for d1 = 1,2 do
   for d2 = 1,3 do
      for d3 = 1,4 do
         for d4=1,5 do
            i = i + 1
            assert(t[d1][d2][d3][d4] == i)
         end
      end
   end
end

checksize(t, 4, 1, 2, 3, 4, 5)

x = t:select(2)
checksize(x, 3, 61, 3, 4, 5)
i = 60
x:apply(function(z)
           i = i + 1
           assert(z == i)
        end)

x = x:select(1)
x = x:narrow(2, 3)

checksize(x, 2, 61+5, 3, 5)
i = 65
x:apply(function(z)
           i = i + 1
           assert(z == i)
        end)

u = torch.Tensor(3*5)
u:fill(3)
x:copy(u)
x:apply(function(z)
           assert(z == 3)
        end)

i = 0
t:apply(function(z)
           i = i + 1
           if i <= 65 or i > 80 then
              assert(z == i)
           else
              assert(z == 3)
           end
        end)

print('**** maths ****')

t:zero()
t:apply(function(z)
           assert(z == 0)
        end)

t:add(66)
t:apply(function(z)
           assert(z == 66)
        end)

x = torch.Tensor(2,3,4,5):fill(3)
t:add(2, x)
t:apply(function(z)
           assert(z == 72)
        end)

t:mul(2)
t:apply(function(z)
           assert(z == 144)
        end)

t:fill(3)
x:fill(2)
t:cmul(x)
t:apply(function(z)
           assert(z == 6)
        end)
t:cdiv(x)
t:apply(function(z)
           assert(z == 3)
        end)

y = torch.Tensor(2*3, 4*5):fill(4)
t:addcmul(5, x, y)
t:apply(function(z)
           assert(z == 3+5*(2*4))
        end)
t:addcdiv(10, y, x)
t:apply(function(z)
           assert(z == 3+5*(2*4)+20)
        end)

x = torch.Tensor(3,4):fill(2)
y = torch.Tensor(3*4):fill(3)
assert(x:dot(y) == 12*2*3)

i = 0
x:apply(function()
           i = i + 1
           return i
        end)
assert(x:min() == 1)
assert(x:max() == 12)
assert(x:sum() == 12+11+10+9+8+7+6+5+4+3+2+1)

x = torch.Tensor(5)
y = torch.Tensor(4)
i = 0
x:apply(function()
           i = i + 1
           return i
        end)
i = 0
y:apply(function()
           i = i + 1
           return i
        end)


z = torch.Tensor(5, 4)
z:fill(1)
z:addr(2, x, y)
for i=1,5 do
   for j=1,4 do
      assert(1+2*x[i]*y[j] == z[i][j])
   end
end

x = torch.Tensor(4)
i = 0
x:apply(function()
           i = i + 1
           return i
        end)
i = 0
z:apply(function(z)
           i = i + 1
           return i
        end)

y = torch.Tensor(5):fill(1)
y:addmv(2, false, z, x)

for i=1,5 do
   local sum = 0
   for j=1,4 do
      sum = sum+z[i][j]*x[j]
   end
   assert(1+2*sum == y[i])
end

x:fill(1)
i = 0
y:apply(function()
           i = i + 1
           return i
        end)
x:addmv(2, true, z, y)

for i=1,4 do
   local sum = 0
   for j=1,5 do
      sum = sum+z[j][i]*y[j]
   end
   assert(1+2*sum == x[i])
end

----- addmm

x = torch.Tensor(3,2)
y = torch.Tensor(2,4)
fillup(x)
fillup(y)
z = torch.Tensor(3,4)
z:fill(1)
z:addmm(2, false, x, false, y)
for i=1,3 do
   for j=1,4 do
      local sum = 0
      for k=1,2 do
         sum = sum + x[i][k]*y[k][j]
      end
      assert(1+2*sum == z[i][j])
   end
end

x = torch.Tensor(2,3)
y = torch.Tensor(2,4)
fillup(x)
fillup(y)
z = torch.Tensor(3,4)
z:fill(1)
z:addmm(2, true, x, false, y)
for i=1,3 do
   for j=1,4 do
      local sum = 0
      for k=1,2 do
         sum = sum + x[k][i]*y[k][j]
      end
      assert(1+2*sum == z[i][j])
   end
end

x = torch.Tensor(3,2)
y = torch.Tensor(4,2)
fillup(x)
fillup(y)
z = torch.Tensor(3,4)
z:fill(1)
z:addmm(2, false, x, true, y)
for i=1,3 do
   for j=1,4 do
      local sum = 0
      for k=1,2 do
         sum = sum + x[i][k]*y[j][k]
      end
      assert(1+2*sum == z[i][j])
   end
end

x = torch.Tensor(2,3)
y = torch.Tensor(4,2)
fillup(x)
fillup(y)
z = torch.Tensor(3,4)
z:fill(1)
z:addmm(2, true, x, true, y)
for i=1,3 do
   for j=1,4 do
      local sum = 0
      for k=1,2 do
         sum = sum + x[k][i]*y[j][k]
      end
      assert(1+2*sum == z[i][j])
   end
end

if torch.typename(z) == 'torch.DoubleTensor' or torch.typename(z) == 'torch.FloatTensor' then
   print('**** stats ****')

   function checkbasicstat(func, mathfunc, delta, zarg)
      delta = delta or 0
      x = torch.Tensor(10)
      i = 0
      x:apply(function()
                 i = i + 1
                 return (i-delta)/11
              end)

      x[func](x, zarg)
      i = 0
--      print(func)
      x:apply(function(z)
                 i = i + 1
--                 print(mathfunc((i-delta)/11), z)
                 assert(math.abs(mathfunc((i-delta)/11) - z) < 0.00001)
              end)
   end

   checkbasicstat('log', math.log)
--    checkbasicstat('log1p', function(z)
--                             return math.log(1+z)
--                          end)
   checkbasicstat('exp', math.exp)
   checkbasicstat('cos', math.cos)
   checkbasicstat('acos', math.acos)
   checkbasicstat('cosh', math.cosh)
   checkbasicstat('sin', math.sin)
   checkbasicstat('asin', math.asin)
   checkbasicstat('sinh', math.sinh)
   checkbasicstat('tan', math.tan)
   checkbasicstat('atan', math.atan)
   checkbasicstat('tanh', math.tanh)
   checkbasicstat('sqrt', math.sqrt)
   checkbasicstat('ceil', math.ceil, -5)
   checkbasicstat('floor', math.floor, -5)
   checkbasicstat('abs', math.abs, 5)
   checkbasicstat('pow', function(z)
                            return z*z
                         end, nil, 2)

   fillup(x)
   assert(x:mean() == x:sum()/x:nElement())
   do
      local sum = 0
      x:apply(function(z)
                 local zz = z - x:mean()
                 sum = sum + zz*zz
              end)
      sum = sum/(x:nElement()-1)
      assert((sum - x:var()) < 0.00001)
      assert((math.sqrt(sum) - x:std()) < 0.00001)
   end
end

do
   x = torch.Tensor(10)
   fillup(x)
   local function norm(z, d)
      local sum = 0
      z:apply(function(zz)
                 sum = sum + math.pow(math.abs(zz), d)
              end)
      sum =  math.pow(sum, 1/d)
      return sum
   end
   assert((norm(x, 2) - x:norm()) < 0.0001)
   assert((norm(x, 1) - x:norm(1)) < 0.0001)
   assert((norm(x, 3) - x:norm(3)) < 0.0001)

   y = torch.Tensor(10)
   fillup(x)
   fillup(y)
   y:add(2)
   z = torch.Tensor(10):zero()
   z:copy(x):add(-1, y)
   assert((norm(z, 2) - x:dist(y)) < 0.0001)
   assert((norm(z, 1) - x:dist(y, 1)) < 0.0001)
   assert((norm(z, 3) - x:dist(y, 3)) < 0.0001)
end


print('PASSED')
