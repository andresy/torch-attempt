require "torch"

torch.Storage = torch.DoubleStorage

s = torch.Storage(10)

s:fill(65.14)

assert(s:size() == 10)
for i=1,10 do
   assert(s[i] == 65.14)
end

s:resize(20)
for i=1,10 do
   assert(s[i] == 65.14)
end

for i=11,20 do
   s[i] = 66
   assert(s[i] == 66)
end

t = torch.Storage(20)
t:copy(s)

for i=1,10 do
   assert(t[i] == 65.14)
end
for i=11,20 do
   assert(t[i] == 66)
end

r = torch.ByteStorage(20)
--r = torch.CharStorage(20)
r:copy(t)
for i=1,10 do
   assert(r[i] == 65)
end
for i=11,20 do
   assert(t[i] == 66)
end

assert(#r == 20)

st = r:string()
print(st)
assert(st == 'AAAAAAAAAABBBBBBBBBB')

print(r)
print(s)
print(t)

print('PASSED')
