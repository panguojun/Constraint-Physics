----------------------------------------------
-- 边界限制
----------------------------------------------
limit = 200
limit_x = {cs = '1ax',x1 = -2,x2 = 2,y1 = 0,y2 = 0,z1 = 0,z2 = 0}
limit_y = {cs = '1ax',x1 = 0,x2 = 0,y1 = -2,y2 = 2,z1 = 0,z2 = 0}
----------------------------------------------
function check_limit(v, limits)
	if(limits.cls == '1ax')then
		return v.x < limits.x1 or v.x > limits.x2 or v.y < limits.y1 or v.y > limits.y2 or v.z < limits.z1 or v.z > limits.z2
	end
	return true
end
function limit_prj(v, g, limits)
	if(limits.cs == '1ax')then
		local nv = v + g;
		if nv.x < limits.x1 or nv.x > limits.x2 then
			g.x = 0;
		end
		if nv.y < limits.y1 or nv.y > limits.y2 then
			g.y = 0;
		end
		if nv.z < limits.z1 or nv.z > limits.z2 then
			g.z = 0;
		end
		norm(g)
	elseif(limits.cs == 'p2p')then	
		if getdis(v + V(g.x,0,0), limits.p1, limits.p2) > 0.01 then
			g.x = 0;
		end
		if getdis(v + V(0,g.y,0), limits.p1, limits.p2) > 0.01 then
			g.y = 0;
		end
		if getdis(v + V(0,0,g.y), limits.p1, limits.p2) > 0.01 then
			g.z = 0;
		end
		norm(g)
	end
end