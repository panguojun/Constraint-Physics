PI = math.pi
------ lua ------
function clone(object)
     local lookup_table = {}
    local function copyObj( object )
        if type( object ) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs( object ) do
            new_table[copyObj( key )] = copyObj( value )
        end
        return setmetatable( new_table, getmetatable( object ) )
    end
    return copyObj( object )
end
------ math ------
function blend(a, b, alpha)
	return a * (1 - alpha) + b * alpha
end
function d6()
	return math.ceil(math.random() * 5 + 1);
end
function d100()
	return math.ceil(math.random() * 99 + 1);
end
function dx(x)
	return math.ceil(math.random() * (x - 1) + 1);
end
function c6()
	if math.ceil(math.random() * 6) == 0 then
		return 1;
	end
	return 0;
end
function rnd(min, max)
	return math.random() * (max - min) + min;
end
function rndi(min, max)
  min = math.ceil(min);
  max = math.floor(max);
  return math.floor(math.random() * (max - min)) + min;
end
function parity(a)
	n, dec = math.modf(a, 2)
	if n == 0 then
		return 1
	else
		return -1
	end
end

----------------------------
--		Draw 2D
----------------------------
function luacircle(x0, y0, r)	
	for i = 0, 200 do
		ang = i / 100 * 3.1416 * 2
		x = x0 + r * math.cos(ang)
		y = y0 + r * math.sin(ang)
		pix(x, y)
	end
end
function lualine(x1, y1, x2, y2)	
	for i = 0, 100 do
		pix(blend(x1, x2, i / 100), blend(y1, y2, i / 100))
	end
end


----------------------------

eops={}
eops.__add=function(e,a)
	table.insert(e,a)
	return e
end
eops.__tostring=function (e)
	local s = '\ne:\n'
	for i=1,#e do
		s = s .. ("x=" .. e[i].x .. ",y=" .. e[i].y)
		s = s .. '\n'
	end
	return s
end
function E(e)
	setmetatable(e, eops)
	return e
end
function TRIANG(p1,p2,p3)
	triang(p1.x,p1.y,0,p2.x,p2.y,0,p3.x,p3.y,0)
end
function FACE(e1, e2)
	newe()
	for i=1,#e1 do
		addv(e1[i].x,e1[i].y,0)
	end
	newe()
	for i=1,#e2 do
		addv(e2[i].x,e2[i].y,1)
	end
	face()
	pop()
	pop()
end

----------------------------
-- Vector3
----------------------------
ops={}
function V(v)
	setmetatable(v, ops)
	return v
end
function V(x,y,z)
	local v={x=x,y=y,z=z}
	setmetatable(v, ops)
	return v
end
ops.__eq=function (a, b)
	if a.x == b.x and a.y == b.y and a.z == b.z then
		return true
	end
	return false
end
ops.__tostring=function (a)
	return("x=" .. a.x .. "y=" .. a.y .. "z=" .. a.z)
end
ops.__add=function (a, b)
	local v=V(x,y,z)
	v.x = a.x + b.x
	v.y = a.y + b.y
	v.z = a.z + b.z
	return v
end
ops.__sub=function(p1, p2)
	local v=V(x,y,z)
	v.x = p1.x - p2.x
	v.y = p1.y - p2.y
	v.z = p1.z - p2.z
	return v
end
ops.__mul=function(p, s)
	local v=V(x,y,z)
	v.x = p.x * s
	v.y = p.y * s
	v.z = p.z * s
	return v
end


----------------------------
--			3D
----------------------------
function dot(v1, v2)
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
end

-- 向量叉乘
function cross(v1, v2)
    return V(v1.y*v2.z - v2.y*v1.z, v2.x*v1.z-v1.x*v2.z, v1.x*v2.y-v2.x*v1.y)
end

-- 向量的模
function length(v)
    return math.sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
end

-- 向量norm
function norm(v)
	local r = length(v)
	if r > 0 then
		v.x=v.x/r
		v.y=v.y/r
		v.z=v.z/r
	end
end
function normcopy(v)
	local r = length(v)
	if r > 0 then
		return V(v.x/r, v.y/r, v.z/r)
	end
	return V(0,0,0)
end

----------------------------
-- COORD
----------------------------
--COORD({o=V(55,0,0),ux=V(1,0,0),uy=V(0,1,0)})
_coord_ops={}
function COORD(v)
	setmetatable(v, _coord_ops)
	return v
end
function COORD(x,y,z)
	local v={o=V(x,y,z),ux=V(1,0,0),uy=V(0,1,0)}
	setmetatable(v, _coord_ops)
	return v
end
_coord_ops.__mul=function(c,p)
	return c.ux * p.x + c.uy * p.y + c.o
end
_coord_ops.__div=function(p, c)
	local v = p - c.o
	return V(dot(v,c.ux), dot(v,c.uy),0);
end
coord_div=function(p, c)
	local v = p - c.o
	return V(dot(v,c.ux), dot(v,c.uy),0);
end
coord_mul=function(p, c)
	return c.ux * p.x + c.uy * p.y + c.o
end
coordgrad_mul=function(p, c)
	return c.ux * p.x + c.uy * p.y
end
----------------------------
--			File
----------------------------
function readfile(filename)
	local file = io.open(filename, "r") 
	local ret = ""
	local line = ""
	for line in file:lines() do
		 ret = ret .. "\n" .. line
	end
	file:close()
	prt("readfile:")
	prt(ret);
	prt("\n");
	return ret;
end

--[[
print_dump是一个用于调试输出数据的函数，能够打印出nil,boolean,number,string,table类型的数据，以及table类型值的元表
参数data表示要输出的数据
参数showMetatable表示是否要输出元表
参数lastCount用于格式控制，用户请勿使用该变量
]]
function print_dump(data, showMetatable, lastCount)
    if type(data) ~= "table" then
        --Value
        if type(data) == "string" then
            io.write("\"", data, "\"")
        else
            io.write(tostring(data))
        end
    else
        --Format
        local count = lastCount or 0
        count = count + 1
        io.write("{\n")
        --Metatable
        if showMetatable then
            for i = 1,count do io.write("\t") end
            local mt = getmetatable(data)
            io.write("\"__metatable\" = ")
            print_dump(mt, showMetatable, count)    -- 如果不想看到元表的元表，可将showMetatable处填nil
            io.write(",\n")     --如果不想在元表后加逗号，可以删除这里的逗号
        end
        --Key
        for key,value in pairs(data) do
            for i = 1,count do io.write("\t") end
            if type(key) == "string" then
                io.write("\"", key, "\" = ")
            elseif type(key) == "number" then
                io.write("[", key, "] = ")
            else
                io.write(tostring(key))
            end
            print_dump(value, showMetatable, count) -- 如果不想看到子table的元表，可将showMetatable处填nil
            io.write(",\n")     --如果不想在table的每一个item后加逗号，可以删除这里的逗号
        end
        --Format
        for i = 1,lastCount or 0 do io.write("\t") end
        io.write("}")
    end
    --Format
    if not lastCount then
        io.write("\n")
    end
end
function dumpv(name, v)
	prt(name .. string.format('%.2f',v.x) .. "," .. string.format('%.2f',v.y) .. "," .. string.format('%.2f',v.z))
end
----------------------------
function string_contains(str, item)
    local t = {}
    local l = {}
    local index = 0
    for i = 1, string.len(str) do
        table.insert(t, string.byte(string.sub(str, i, i)))
    end

    for i = 1, string.len(item) do
        table.insert(l, string.byte(string.sub(item, i, i)))
    end
    if #l > #t then
        return false
    end

    for k, v1 in pairs(t) do
        index = index + 1
        if v1 == l[1] then
            local iscontens = true
            for i = 1, #l do
                if t[index + i - 1] ~= l[i] then
                    iscontens = false
                end
            end
            if iscontens then
                return iscontens
            end
        end
    end
    return false
end
----------------------------
formlist = {} -- UI Container
----------------------------
function addbutton(fm)
	fm.x = 900 - #formlist * 150
	fm.y = 0
	table.insert(formlist,fm)
end
----------------------------
-- curve
----------------------------
curvedat = {}
function addpt(_x, _y)
	table.insert(curvedat, {x=_x, y=_y})
end

-- Lua table deep copy
function clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end