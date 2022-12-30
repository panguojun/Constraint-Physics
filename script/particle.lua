----------------------------------------------
-- PE(p,v)
----------------------------------------------
PE_ops = {}
function PE(v)
	setmetatable(v, PE_ops)
	return v
end
function PE(p,v)
	local v={p=p, v=v, r=18,th=0}
	setmetatable(v, PE_ops)
	return v
end
function drawPE(pe)
	pe.p.x = pe.p.x + pe.v.x * 0.01
	pe.p.y = pe.p.y + pe.v.y * 0.01
	circle(pe.p.x,pe.p.y, pe.r)
	arrow(pe.p.x,pe.p.y,pe.p.x+pe.v.x,pe.p.y+pe.v.y)
end
function drawPE_C(pe, C)
	pe.p.x = pe.p.x + pe.v.x * 0.001
	pe.p.y = pe.p.y + pe.v.y * 0.001
	local p = coord_Polar_CartesianV(pe.p)
	local np = coord_Polar_CartesianV(pe.p+pe.v * 0.01)
	circle(200 + p.x,200 + p.y, pe.r)
	arrow(200 + p.x,200 + p.y,200 + np.x,200 + np.y)
end