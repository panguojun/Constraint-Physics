--[[ ***************************************************
					在约束中定义形状
					从特征空间映射得到属性
					还原形状:
					给定一个起始点，特征向量，计算一个终点
--]]
-- 圆弧
--v: r,th,0
function v_arc(v)
	local r = v.x
	local ang = v.y*math.pi/180

	return V(r * math.cos(ang),r * math.sin(ang),0)
end
-- 线段
-- v: vx,vy,l
function v_sigment(v)
	local dir = VEC(v.x,v.y,0)
	local l = v.z
	return V(dir.x * l,dir.y * l,0)
end