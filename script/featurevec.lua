--[[ *******************************************************
					 特征空间
				借用了人工智能的概念
				把数据进行多维向量化抽象
				然后建立一个向量空间
				这个向量空间支持运算，组成一个群
				向量可以移动，变形，向量之间可以融合
				这里使用LUA来实现，利用了LUA复杂性方面的优势
]]

----------------------------
-- Vector
----------------------------
_vec_ops={}
function VEC(v)
	setmetatable(v, _vec_ops)
	return v
end
function VEC(x,y,z)
	local v={x=x,y=y,z=z}
	setmetatable(v, _vec_ops)
	return v
end
_vec_ops.__eq=function (a, b)
	if a.x == b.x and a.y == b.y and a.z == b.z then
		return true
	end
	return false
end
_vec_ops.__tostring=function (a)
	return("x=" .. a.x .. "y=" .. a.y .. "z=" .. a.z)
end
_vec_ops.__add=function (a, b)
	local v=V(x,y,z)
	v.x = a.x + b.x
	v.y = a.y + b.y
	v.z = a.z + b.z
	return v
end
_vec_ops.__sub=function(p1, p2)
	local v=V(x,y,z)
	v.x = p1.x - p2.x
	v.y = p1.y - p2.y
	v.z = p1.z - p2.z
	return v
end
_vec_ops.__mul=function(p, s)
	return V( p.x * s, p.y * s, p.z * s)
end

function movVEC(v,index,delta)
	return movep({x=v.x,y=v.y,z=v.z},index,delta);
end
