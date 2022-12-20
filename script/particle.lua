----------------------------------------------
-- P(p,g,v)
----------------------------------------------
P1 = {
	p = V(100,100),
	g = V(0.5,1),
	v = V(0.2,0.01), 
	L = 100, T = 100
}
moveXY = function (P)
	return {p = P.p + P.g,g = P.g,v = P.v,L = P.L, T = P.T}
end
moveT=function (P, dt)
	return {p = P.p + P.v * dt,g = P.g,v = P.v,L = P.L, T = P.T}
end
breath = function (P)
	P = moveT(P, 1)
	local PP = clone(P)
	for u = 0, PP.T do
		PP = moveXY(PP)
		local cp = CF_link_p_1({angstep = 5}, PP.p) * 0.5
		cp = coord_mul(cp, coord2)
		pix(cp.x,cp.y)
	end
	return P
end