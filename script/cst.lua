----------------------------------------------
-- 约束特化硬代码
-- 类型化，数值化
----------------------------------------------
require ("script/cstframe")
----------------------------------------------
-- 常数与特征控制变量
----------------------------------------------
mindis = 8
targetP = V(0,0,0)

-- 粒子定义，暂时赋予它一个位置
-- 特征向量与坐标系结合，是形状
startA = {o=V(0,0,0),v=V(0,0,0)}
veclist = {}

----------------------------------------------
-- 坐标转换
----------------------------------------------
-- 极坐标系转换函数
function CF_link_p_1(C,p)
	local ang = math.rad(p.y*C.angstep)
	return V(p.x * math.cos(ang), p.x * math.sin(ang),0)
end
----------------------------------------------
-- 距离函数
----------------------------------------------
dis_fun = function (cst,vc,target)
	return length(coord_mul(cst.transform(cst,vc), cst.C)-target)
end
dis_fun2 = function (cst,vc,target)
	-- 位置距离
	local pw = coord_mul(cst.transform(cst,vc), cst.C)
	local pdis = math.abs(length(pw-target) - 80) -- C2.R
	-- 方向距离
	local pdir = normcopy(pw - cst.C.o)
	local tdir = normcopy(pw - target)
	local dis = math.abs(dot(pdir,tdir))*80
	prt("pdis = " .. pdis .. " dis= " .. dis)
	return dis + pdis
end

----------------------------------------------
-- 约束对象定义
----------------------------------------------
link_cst.transform = CF_link_p_1
link_cst.angstep = 5
----------------------------------------------
-- 杆对象
link_cst1 = clone(link_cst)
link_cst1.transform = CF_link_p_1
link_cst1.label = 'C1'
link_cst1.angstep = 1
link_cst1.step = 10
link_cst1.place = function(Vw,cst)
		dumpv("place------------------", Vw)
		psz(6)
		pix(cst.C.o.x, cst.C.o.y)
		text(cst.C.o.x, -20+cst.C.o.y, 'C1')
		psz(1)
		return {o=V(100, 0, 0),v=V(0, 0, 0)};
	end
link_cst1.move = function(v, dir)
		if(dir == 2)then
			return V(v.x,v.y+1,v.z);
		elseif(dir == -2)then
			return V(v.x,v.y-1,v.z);
		end
		return V(v.x,v.y,v.z);
	end
link_cst1.dump = function(cst)
		local v = V(100,0,0)
		local vx = v
		for y= 0, 80 do
			vx = cst.move(vx, -2)
			local vv= coord_mul(cst.transform(cst,vx), cst.C)
			pset(vv.x,vv.y)
		end
		vx = v
		for y= 0, 80 do
			vx = cst.move(vx, 2)
			local vv= coord_mul(cst.transform(cst,vx), cst.C)
			pset(vv.x,vv.y)
		end
	end
----------------------------------------------
-- 圆约束
link_cst2 = clone(link_cst)
link_cst2.transform = CF_link_p_1
link_cst2.label = 'C2' 
link_cst2.steps = 40
link_cst2.angstep = 1
link_cst2.step = 5
link_cst2.place = function(Vw,cst)
		dumpv("place------------------", Vw)
		psz(6)
		pix(cst.C.o.x, cst.C.o.y)
		text(cst.C.o.x, -20+cst.C.o.y, 'C2')
		psz(1)
		return {o=V(80, 0, 0),v=V(0, 0, 0)};
	end
link_cst2.move = function(v, dir)
	if(dir == 2)then
		return V(v.x,v.y+1,v.z);
	elseif(dir == -2)then
		return V(v.x,v.y-1,v.z);
	end
	return V(v.x,v.y,v.z);
end
link_cst2.fly= function(v,cst)
		local vv= coord_mul(cst.transform(cst,v), cst.C)
		dumpv("fly====================", cst.C.o)
		return {o=vv,v=V(0, 0, 0)}
	end
link_cst2.dump = function(cst)
		local v = V(80,0,0)
		local vx = v
		for y= 0, 80 do
			vx = cst.move(vx, -2)
			local vv= coord_mul(cst.transform(cst,vx), cst.C)
			pset(vv.x,vv.y)
		end
		vx = v
		for y= 0, 80 do
			vx = cst.move(vx, 2)
			local vv= coord_mul(cst.transform(cst,vx), cst.C)
			pset(vv.x,vv.y)
		end
	end

----------------------------------------------
-- 杆对象
link_cst3 = clone(link_cst)
link_cst3.label = 'C3'
----------------------------------------------
-- dump
----------------------------------------------
function dump_cst()
	link_cst1.dump(link_cst1)
	link_cst2.dump(link_cst2)
	link_cst3.dump(link_cst3)
end
