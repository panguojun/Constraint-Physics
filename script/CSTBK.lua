
----------------------------------------------
-- 梯度，力即梯度
----------------------------------------------
function _grad1(c, d_fun, target, force)
	local fo  = d_fun(V(c.x,c.y,c.z), target)
	if(fo < mindis) then
		return nil
	end
	local fdx = d_fun(V(c.x+1,c.y,c.z), target)
	local dfx = fdx - fo
	return normcopy(V(-dfx,0,0))
end
function _grad3(c, d_fun, target, force)
	local fo  = d_fun(V(c.x, c.y,c.z), target)
	if(fo < mindis) then
		return nil
	end
	local fdx = d_fun(V(c.x+1,c.y,c.z), target)
	local fdy = d_fun(V(c.x,c.y+1,c.z), target)
	local fdz = d_fun(V(c.x,c.y,c.z+1), target)
	local dfx = fdx - fo
	local dfy = fdy - fo
	local fdz = fdz - fo
	return normcopy(V(-dfx,-dfy,-fdz))
end
----------------------------------------------
-- 广义的运动(可以在特征空间“运动”)，物体在约
-- 束跟力的共同作用下，也可以是变形
----------------------------------------------
function v_add_grad(v, grad)
	if(grad == nil)then
		prt("grad nil!")
		return v
	end
	dumpv(grad)
	return v + grad
end
function grad_add_grad(v, g1, lim1, g2, lim2)
	if g1 == nil then
		prt("!g1")
		return g2
	end
	if g2 == nil then
		prt("!g1")
		return g1
	end
	local g = g1 + g2
	limit_prj(v,g,lim1)
	limit_prj(v,g,lim2)
	return g;
end
function grad(v, d_fun, target, gradfun, lim_set)
	if(is_back(v)) then
		return nil
	end
	local g = gradfun(v, d_fun, target)
	if g == nil then
		return nil
	end
	limit_prj(v,g,lim_set)
	dumpv(g)
	return g
end
function move0(v, d_fun, target, gradfun,lim_set)
	local g = grad(v, d_fun, target, gradfun,lim_set)
	if g == nil then
		return v
	end
	dumpv(g)
	v = {x=v.x+g.x,y=v.y+g.y,z=v.z+g.z};
	return v
end