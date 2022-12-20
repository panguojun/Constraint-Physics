----------------------------------------------
-- APP
----------------------------------------------
require ("script/cst")
----------------------------------------------


----------------------------------------------
-- onGUI
----------------------------------------------
angle = 0
function onGUI()
	gui = 0
	if(button1(450, 400, "test")) then
		
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
	angle = angle + 5
	--rgb(255,255,255)
	--breath(P1)
	setcd3("c1",V(1,0,0),V(0,1,0),V(0,0,1));
	rotcd3("c1",angle, normcopy(V(1,1,0)));
	for i = 0, 10 do
		local x,y,z = trans(V(i*10,10,10), "c1")
		pix(200 +x,200 +y)
	end
	for i = 0, 10 do
		local x,y,z = trans(V(10,i*10,10), "c1")
		pix(200 +x,200 +y)
	end
end