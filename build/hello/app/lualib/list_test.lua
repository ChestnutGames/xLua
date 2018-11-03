local list = require "list"

local l = list()

print('test::list::foreach')
l:foreach(function (i, ... )
	-- body
	assert(i)
end)

print('test::list::foreach')
for i=1,100 do
	l:push_back({ i = i })
end

print('test::list::foreach')
l:foreach(function (i, ... )
	-- body
	print(i.i)
end)

for i=1,200 do
	l:pop_front()
end

l:foreach(function (i, ... )
	-- body
	print(i.i)
end)