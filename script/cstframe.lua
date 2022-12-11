----------------------------------------------
-- 通用约束框架
-- 约束的复杂性部分实现
----------------------------------------------
text(250,200, "Everything is Constaints!")
----------------------------------------------
require ("script/com")
require ("script/featurevec")
require ("script/lim")
require ("script/dis")
require ("script/vecshape")
require ("script/cst_fit")
require ("script/ui")

----------------------------------------------
-- 约束的通用骨架
----------------------------------------------
link_cst = {
	label = 'C',
	-- 坐标系变换
	C={o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)},
	-- 移动步数
	steps = 20,
	-- 移动步长
	step = 5,
	-- 角度步长
	angstep = 5,
	-- 代表转化为参数
	place = function(Vw,cst)
		dumpv("place------------------", Vw)
		psz(6)
		pix(cst.C.o.x, cst.C.o.y)
		text(cst.C.o.x, -20+cst.C.o.y, cst.label)
		psz(1)
		return {o=V(0, 0, 0),v=V(0, 0, 0)};
	end,
	-- 代表转化为形状
	fly= function(v,cst)
		local vv= coord_mul(cst.transform(cst,v), cst.C)
		dumpv("fly====================", cst.C.o)
		arrow(cst.C.o.x,cst.C.o.y,vv.x,vv.y)-- 绘制
		return {o=vv,v=V(0, 0, 0)}
	end,
	--参数移动
	move = function(v, dir)
		if(dir == 1)then
			return V(v.x+10,v.y,v.z);
		elseif(dir == 2)then
			return V(v.x,v.y+1,v.z);
		elseif(dir == -2)then
			return V(v.x,v.y-1,v.z);
		end
		return V(v.x,v.y,v.z);
	end,
	-- dump
	dump = function(cst)
		local v = V(0,0,0)
		for x= 0, 10 do
			v = cst.move(v, 1)
			local vx = v
			for y= 0, 40 do
				vx = cst.move(vx, -2)
				local vv= coord_mul(cst.transform(cst,vx), cst.C)
				pset(vv.x,vv.y)
			end
			for y= 0, 40 do
				vx = cst.move(vx, 2)
				local vv= coord_mul(cst.transform(cst,vx), cst.C)
				pset(vv.x,vv.y)
			end
		end
	end
}
----------------------------------------------
-- 把向量放在约束空间内
----------------------------------------------
function world_cst(Vw, cst)
	return cst.place(Vw.o,cst)
end
function cst_world(Vc, cst)
	return cst.fly(Vc.o,cst)
end
----------------------------------------------
function grad(p, C, d_fun, target)
	local d  = d_fun(C, p,target)
	if(d < mindis) then
		prt("!! d < mindis d=" .. d .. " mindis=" .. mindis)
		return nil
	end
	local fdx  = d_fun(C, C.move(p, 1),target)
	local fdx_ = d_fun(C, C.move(p, -1),target)
	local fdy  = d_fun(C, C.move(p, 2),target)
	local fdy_ = d_fun(C, C.move(p, -2),target)
	local dfx = ((fdx - d) - (fdx_ - d)) / 2
	local dfy = ((fdy - d) - (fdy_ - d)) / 2
	
	prt("d =" .. d .. " dfx=" .. dfx .. " ,dfy=" .. dfy)
	-- 只有一个主方向,一次多动一些
	return normcopy(V(-dfx,-dfy,0))*C.step
end
function move1(A, cst, d_fun, target)
	local p = A.o;
	local pp = coord_mul(cst.transform(cst,p), cst.C);
	psz(2);pix(pp.x, pp.y);psz(1)
	local g = grad(p, cst, d_fun, target)
	if g == nil then
		return nil
	end
	return {o={x=p.x+g.x,y=p.y+g.y,z=p.z+g.z},v=A.v};
end
function move(A, C, d_fun,target)
	local ta = A
	for i = 0, C.steps do
		local ta2 = move1(ta, C, d_fun, target)
		if ta2 == nil then
			return ta
		end
		ta = ta2
	end
	return ta
end