----------------------------------------------
-- GUI 跟逻辑深度结合
-- Controls
----------------------------------------------
function button1(x, y, label)
	local w = 80
	local h = 30

    local mx,my,mz= getvec3("mouse")
	if(mx > x and mx < x + w and my > y and my < y + h) then
		rect(x-3,y-3,86,36)
		text(x + 20,y + 10, label .. "!")
		return true
	end 
	rect(x,y,80,30)
	text(x + 20,y + 10, label)
	return false	
end

----------------------------------------------
-- UI
----------------------------------------------
count = 1
bshow = true
function onlbtn()
	coord1 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}
	coord2 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}
	coord3 = {o=V(0,0,0),ux=V(1,0,0),uy=V(0,1,0)}

	if onGUI() then
		return
	end

	veclist[count]= {o=V(getvec3("mouse")),v=V(0,0,0)}
	count = count + 1
	if(count > 4)then
		count = 4
	end
	if veclist then
		psz(4)
		for k,v in pairs(veclist) do
			pix(v.o.x, v.o.y)
		end
		psz(1)
	end

	local x,y,z = getvec3("mouse")
	text(x, y - 20,'T')

	if bshow then
		dophg("script/test.txt")
	end
end
function onGUI()
	local hit = false
	gui = 0
	if(button1(650, 50, "path")) then
		--dump_cst()
		--dophg("script/test.txt")
		local a = veclist[1].o
		local b = veclist[2].o
		path(a,b)
		hit = true
		gui = 1
		count = 1
		veclist = {}
	end
	if(button1(650, 150, "reset")) then
		gui = 2
		if count >= 1 and #veclist > 1 then
			startA.o = veclist[1].o
			coord1.o = veclist[1].o
		end
		if count >= 2 and #veclist > 2 then
			coord2.o = veclist[2].o
		end
		if count >= 3 and #veclist > 3 then
			coord3.o = veclist[3].o
		end
		count = 1
		veclist = {}
		hit = true
		bshow = true
		dophg("script/test.txt")
	end
	if button1(650, 250, "clear") then
		gui = 3
		count = 1
		veclist = {}
		hit = true
		bshow = false
	end
	if button1(650, 360, "exit") then
		gui = 4
		exit()
	end
	if button1(350, 360, "play") then
		gui = 18
		play()
		hit = true
		bshow = false
	end
	return hit
end
function onmouse()
	local x,y,z = getvec3("mouse")
	pix(x, y)
	text(x, y - 20,'A')
	arrow(x,y,x,y + 50);
end