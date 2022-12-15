----------------------------------------------
-- 约束特化硬代码
-- 类型化，数值化
----------------------------------------------
require ("script/cstframe")
----------------------------------------------
-- 常数与特征控制变量
----------------------------------------------
-- 粒子定义，位置 + 向量（本质上是一阶空间向量 + 二阶空间向量）
startA = {o=V(0,0,0),v=V(0,0,0)}
veclist = {}

-- 坐标系对象
coord1 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}
coord2 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}
coord3 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}

----------------------------------------------
-- 坐标转换
----------------------------------------------
-- 极坐标系转换函数
function CF_link_p_1(C,p)
	local ang = math.rad(p.y*C.angstep)
	return V(p.x * math.cos(ang), p.x * math.sin(ang),0)
end
function _CF_link_p_1(C,p)
	local ang = math.deg(math.atan(p.y, p.x))/C.angstep
	local r = length(p)
	return V(r, ang, 0)
end

----------------------------------------------
-- 自然坐标系
----------------------------------------------
-- 永远指向起点的自然坐标系
function nc_1(vc,cst,cd)
	local pw = coord_mul(cst.transform(cst,vc), cd)
	if(length(pw - cd.o) > 0.5) then
		cd.ux = normcopy(cst.cd0.o - pw)
		cd.uy = V(-cd.ux.y,cd.ux.x,0)
		cd.o = pw
		--arrow(pw.x,pw.y,(pw + cd.ux*10).x,(pw +cd.ux*10).y)
		--arrow(pw.x,pw.y,(pw + cd.uy*10).x,(pw +cd.uy*10).y)
		return true
	end
	return false
end

----------------------------------------------
-- 距离函数
----------------------------------------------
-- 普通的两点距离公式
dis_fun = function (cst,cd,cdtrans, vc,target)
	return length(coord_mul(cst.transform(cst,vc), cd)-target)
end
-- 在自然坐标系下，目标点的投影跟固定点的距离
dis_fun2 = function (cst,cd,cdtrans,vc,target)
	local ccd = {o=cd.o,ux=cd.ux,uy=cd.uy}
	local disw = dis_fun(cst,cd,cdtrans,vc,target.cd.o)
	if(disw + cst.mindis < target.r)then
		prt('in dis!')
		text(ccd.o.x, ccd.o.y, 'indis')
		return 10000
	end
	if(false == cdtrans(vc,cst,ccd))then
		return disw
	end
	local tgc = coord_div(target.cd.o,ccd)
	local dis =  length(tgc - V(0, target.r, 0))
	return dis
end
-- 沿半径方向的距离
dis_radious = function (cst,cd,cdtrans,vc,target)
	-- 位置距离
	local pw = coord_mul(cst.transform(cst,vc), cd)
	-- todo 需要投影
	return length(pw-target)
end
-- 角度差
dis_angle = function(cst,cd,cdtrans,vc,target)
	-- 角度距离
	local pw = coord_mul(cst.transform(cst,vc), cd)
	local v1 = (target-cd.o)
	local v2 = (pw-cd.o)
	local ang = angleV2(normcopy(v1), normcopy(v2))
	return math.abs(math.deg(ang))
end

----------------------------------------------
-- 各种类型的约束定义
----------------------------------------------
-- 杆对象(r,th)
link_cst.transform = CF_link_p_1
link_cst.angstep = 5
link_cst.step = 5
link_cst.steps = 20

----------------------------------------------
-- 杆对象(与圆相切)
link_cst_t = clone(link_cst)
link_cst_t.angstep = 18
link_cst_t.step = 1
link_cst_t.steps = 50
link_cst_t.place = function(label,Vw,cst,cd)
		prt(label)
		dumpv("place------------------", Vw)
		psz(6);pix(cd.o.x, cd.o.y);text(cd.o.x, -20+cd.o.y, label);psz(1)
		
		cd.uy = normcopy(cst.cd.o - cst.target.cd.o)
		cd.ux = V(cd.uy.y,-cd.uy.x,0)
		drawCD(cd)
		return {o=V(0, 0, 0),v=V(0, 0, 0)};
	end
link_cst_t.move = function(v, dir, cst)
		if(dir == 1)then
			return V(v.x+1,v.y,v.z);
		elseif(dir == 2)then
			return V(v.x,v.y+1,v.z);
		elseif(dir == -2)then
			return V(v.x,v.y-1,v.z);
		end
		return V(v.x,v.y,v.z);
	end
link_cst_t.fly= function(v,cst,cd)
		local vv= coord_mul(cst.transform(cst,v), cd)
		dumpv("fly====================", vv)
		arrow(cst.cd0.o.x,cst.cd0.o.y,vv.x,vv.y)-- 绘制
		return {o=vv,v=V(0, 0, 0)}
	end

----------------------------------------------
-- 杆对象(r)
link_cst_r = clone(link_cst)
link_cst_r.transform = CF_link_p_1
link_cst_r.angstep = 1
link_cst_r.step = 10
link_cst_r.steps = 50
link_cst_r.mindis = 20
link_cst_r.place = function(label,Vw,cst,cd)
		local Vc = _CF_link_p_1(cst, coord_div(Vw, cd))
		dumpv("placeC------------------", Vc)
		psz(6);pix(cd.o.x, cd.o.y);text(cd.o.x, -28+cd.o.y, label);psz(1)
		return {o=Vc,v=V(0, 0, 0)};
	end
link_cst_r.move = function(v, dir, cst)
		if(dir == 1)then
			return V(v.x+20,v.y,v.z);
		end
		return V(v.x,v.y,v.z);
	end

----------------------------------------------
-- 圆约束(th)
link_cst_th = clone(link_cst)
link_cst_th.transform = CF_link_p_1
link_cst_th.r = 80
link_cst_th.steps = 50
link_cst_th.angstep = 5
link_cst_th.step = 1
link_cst_th.mindis = 4
link_cst_th.place = function(label,Vw,cst,cd)
		local Vc = _CF_link_p_1(cst, coord_div(Vw, cd))
		dumpv("place------------------", Vc)
		psz(6);pix(cd.o.x, cd.o.y);text(cd.o.x, -20+cd.o.y, label);psz(1)
		circle(cd.o.x,cd.o.y, cst.r)
		return {o=Vc,v=V(0, 0, 0)};
	end
link_cst_th.move = function(v, dir, cst)
		if(dir == 2)then
			return V(v.x,v.y+1,v.z);
		elseif(dir == -2)then
			return V(v.x,v.y-1,v.z);
		end
		return V(v.x,v.y,v.z);
	end
link_cst_th.fly= function(v,cst,cd)
		local vv= coord_mul(cst.transform(cst,v), cd)
		dumpv("fly====================", cd.o)
		arrow(cst.cd0.o.x,cst.cd0.o.y,vv.x,vv.y)-- 绘制
		return {o=vv,v=V(0, 0, 0)}
	end