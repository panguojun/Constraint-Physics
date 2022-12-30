----------------------------------------------
-- APP
----------------------------------------------
require ("script/cst")
require ("script/particle")
----------------------------------------------
gui = 0
stage = 0
mousedownx = 0
mousedowny = 0
particles = {}
pelist = {}

-- ×ø±êÏµ±ä»»
function coord_Polar_Cartesian(x, y, z)
	ang = y * 3.1416 * 2 / 100
	return V(x * math.cos(ang),x * math.sin(ang), z)
end
function coord_Polar_CartesianV(v)
	ang = v.y * 3.1416 * 2 / 100
	return V(v.x * math.cos(ang),v.x * math.sin(ang), v.z)
end
function circle2(r,h, cst)	
	local x,y,z;
	for i = 0, 100 do
		x,y,z = fly(coord_Polar_Cartesian(r,i,h), cst)
		pix(x, y)
	end
end
function cylinder(p, cst)
	circle2(p.r, 0,cst);
	circle2(p.r, 100,cst);

	local p1 = V(fly(coord_Polar_Cartesian(0,0,0), cst))
	local p2 = V(fly(coord_Polar_Cartesian(0,0,100), cst))
	line(p1.x,p1.y,p2.x,p2.y)
end

----------------------------------------------
function onlbtn()
	if(stage == 0)then
		return
	end
	mousedownx,mousedowny,z = getvec3("mouse")
	if onGUI() then
		bdrawing = false
		return
	end
	bdrawing = true
end
----------------------------------------------
function onlbtnup()
	if(stage == 0)then
		stage = 1
		cls()
		if onGUI() then
			return
		end
		return
	end
	if false == bdrawing then
		return
	end
	local x,y,z = getvec3("mouse")
	if gui == 1 then
		rgb(255,255,255)
		arrow(mousedownx,mousedowny,x,y)
	end
	if gui == 3 then
		rgb(255,255,255)
		cd3("c1", V(x,y,0))
		rotcd3("c1", 45, V(1,1,0))
		--local pe = {x=mousedownx,y=mousedowny,vx = 0.001,vy = 0,
					--r=dis(V(mousedownx,mousedowny,0),V(x,y,0)),th = 0,dr=0.1, dth = 0.5}
		--cylinder(pe, "c1")
		--particles[#particles + 1] = pe
		
		local pe = PE(V(mousedownx,mousedowny,0), V(0,y-mousedowny,0))
		pelist[#pelist+1] = pe
		
		for k,v in pairs(pelist) do
			drawPE(v)
		end
	end
	if gui == 4 then
		drawCD({o=V(mousedownx,mousedowny,0),ux=V(1,0,0),uy=V(0,1,0)})
	end
end
----------------------------------------------
-- onGUI
----------------------------------------------
function onGUI()
	ret = false
	rgb(158,158,158)
	rect(50,10,680,380)
	if(button(250, 400, "ptc")) then
		pelist = {}
		gui = 3
		ret = true
	end
	
	if(button(450, 400, "back")) then
		cls()
		dolua("script/main.lua")
		gui = 1
		ret = true
		return ret
	end
	if(button(550, 400, "exit")) then
		gui = 2
		ret = true
		exit()
	end
	if(button(150, 400, ">")) then
		cd3("c1",V(1,0,0),V(0,1,0),V(0,0,1));
		play(1000)
		ret = true
	end
	return ret
end
----------------------------------------------
-- onmouse
----------------------------------------------
function onmouse()
	local p = V(getvec3("mouse"))
	if gui == 2 then
		pix(p.x, p.y)
	end
end
----------------------------------------------
-- onplay
----------------------------------------------
function onplay()
	cls()
	for k,v in pairs(particles) do
		local cst = "c"..k
		cd3(cst, V(v.x,v.y,0))
		rotcd3(cst, 0.1, V(1,1,0))
		cylinder(v, cst)
	end
	for k,v in pairs(pelist) do
		drawPE_C(v,coord_Polar_CartesianV)
	end
end
onGUI()