function fit(V, C, d_fun, depth)
	local v = V.v;
	if(depth > maxdepth)then
		return false
	end
	if(is_back(v)) then
		return false
	end
	table.insert(hismap, v)
	dumpv(v)
	
	lambda("g")
	local g = grad_(v, C, d_fun)
	if g == nil then
		return true
	end
	fit({c=V.c,v={x=v.x+g.x,y=v.y+g.y,z=v.z+g.z}}, C, d_fun, depth+1)
	
	return false
end