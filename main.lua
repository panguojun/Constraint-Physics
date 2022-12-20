----------------------------------------------
-- APP
----------------------------------------------
require ("script/cst")
----------------------------------------------
rgb(1,1,1)
text(280,180, "* Constaint Physics *")
----------------------------------------------
P1 = {
	p = V(100,100),
	g = V(0.5,1),
	gt = V(0.2,0.01), 
	L = 100, T = 100
}
moveXY = function (P)
	return {p = P.p + P.g,g = P.g,gt = P.gt,L = P.L, T = P.T}
end
moveT=function (P, dt)
	return {p = P.p + P.gt * dt,g = P.g,gt = P.gt,L = P.L, T = P.T}
end
breath = function (P)
	P = moveT(P, 1)
	local PP = clone(P)
	for u = 0, PP.T do
		PP = moveT(PP,1)
		PP = moveXY(PP)
		local cp = CF_link_p_1({angstep = 5}, PP.p) * 0.5
		cp = coord_mul(cp, coord2)
		pix(cp.x,cp.y)
	end
	return P
end
----------------------------------------------
-- onGUI
----------------------------------------------
function onGUI()
	gui = 0
	if(button1(450, 400, "test")) then
		--rgb(255,255,255)
		--breath(P1)
		local e = PHASE(45)
		dumpv("0------ ", e * V(1,0,0))
		return true
	end
end
----------------------------------------------
-- onmouse
----------------------------------------------
function onmouse()
	local x,y,z = getvec3("mouse")
	--pix(x, y)
	--text2(x, y - 20,'A')
	--arrow(100,200,x,y + 50);
	coord2.o = V(x,y)
	rgb(255,255,255)
	P1 = breath(P1)
end