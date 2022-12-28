----------------------------------------------
-- 通用约束框架
-- 约束的复杂性部分实现
----------------------------------------------
require ("script/com")
require ("script/ui")

----------------------------------------------
-- link类约束的通用原型
----------------------------------------------
link_cst = {
	label = 'C',
	mindis = 10,
	-- 坐标系变换
	C={o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)},
	-- 移动步数
	steps = 20,
	-- 移动步长
	step = 1,
	-- 角度步长
	angstep = 5,
	-- 量度世界坐标系下的向量，转化为参数
	place = function(label,Vw,cst,cd)
		dumpv(label .. " place------------------", Vw);
		psz(6);pix(cd.o.x, cd.o.y);text2(cd.o.x, -20+cd.o.y, label);psz(1)
		return {o=V(0,0,0),v=V(0,0,0)};
	end,
	--参数移动，如果可以合并，则使用线性运算，否则调用寻路函数完成
	move = function(v, dir, cst)
		if(dir == 1)then
			return V(v.x+2,v.y,v.z);
		elseif(dir == 2)then
			return V(v.x,v.y+1,v.z);
		elseif(dir == -2)then
			return V(v.x,v.y-1,v.z);
		end
		return V(v.x,v.y,v.z);
	end,
	-- 还原到世界坐标系，也是转化为形状的过程
	fly= function(v,cst,cd)
		local vv= coord_mul(cst.transform(cst,v), cd)
		dumpv("fly====================", vv)
		arrow(cd.o.x,cd.o.y,vv.x,vv.y)-- 绘制
		return {o=vv,v=V(0, 0, 0)}
	end
}
----------------------------------------------
-- 约束空间 <-> 世界空间
----------------------------------------------
function world_cst(label, Vw, cst, cd)
	if Vw == nil or cst.gui ~= gui then
		return nil
	end
	return cst.place(label,Vw.o, cst, cd)
end
function cst_world(Vc, cst,cd)
	if(Vc == nil)then
		return nil
	end
	return cst.fly(Vc.o,cst,cd)
end
----------------------------------------------
-- move / 寻路算法
----------------------------------------------
function _grad(p, C, CD, cdtrans, d_fun, target)
	local d  = d_fun(C, CD, cdtrans, p, target)
	if d >= 10000 then
		return nil
	end
	if(d < C.mindis) then
		prt("goal!\n")
		drawCD(CD)
		return nil
	end
	local fdx  = d_fun(C, CD, cdtrans, C.move(p, 1, C),	target)
	local fdx_ = d_fun(C, CD, cdtrans, C.move(p, -1, C),target)
	local fdy  = d_fun(C, CD, cdtrans, C.move(p, 2, C),	target)
	local fdy_ = d_fun(C, CD, cdtrans, C.move(p, -2, C),target)
	local dfx = ((fdx - d) - (fdx_ - d)) / 2
	local dfy = ((fdy - d) - (fdy_ - d)) / 2
	--prt("d =" .. d .. " dfx=" .. dfx .. " ,dfy=" .. dfy)
	-- 只有一个主方向
	return normcopy(V(-dfx,-dfy,0)) * C.step
end
function _move(A, cst, cd, cdtrans, d_fun, target)
	local p = A.o;
	local pp = coord_mul(cst.transform(cst,p), cd);
	psz(2);pix(pp.x, pp.y);psz(1)
	local g = _grad(p, cst, cd, cdtrans, d_fun, target)
	if g == nil then
		return nil
	end
	if(nil ~= lastpp)then
		local deta = length(lastpp-pp)
		--prt("deta=" .. deta)
		if(deta > 0.01)then
			g = g * (1 / (deta+0.001))
		end
	end
	lastpp = pp
	return {o={x=p.x+g.x,y=p.y+g.y,z=p.z+g.z},v=A.v};
end
function move(A, CS, CD, cdtrans, d_fun,target)
	if(A == nil)then
		return nil
	end
	lastpp = nil
	CS.cd0 = clone(CD)
	local ta = A
	for i = 0, CS.steps do
		local ta2 = _move(ta, CS, CD, cdtrans, d_fun, target)
		if ta2 == nil then
			return ta
		elseif cdtrans ~= nil then
			cdtrans(ta2.o, CS, CD)
		end
		ta = ta2
	end
	local pp = coord_mul(CS.transform(CS,ta.o), CD);
	text2(pp.x, pp.y, 'failed')
	prt("!failed!\n")

	return nil
end