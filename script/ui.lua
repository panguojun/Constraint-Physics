----------------------------------------------
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
	if(button1(650, 50, "dump")) then
		dump_cst()
		dophg("script/test.txt")
		hit = true
	end
	if(button1(650, 150, "reset")) then
		if count >= 1 and #veclist > 1 then
			link_cst1.C.o = veclist[1].o
		end
		if count >= 2 and #veclist > 2 then
			link_cst2.C.o = veclist[2].o
		end
		if count >= 3 and #veclist > 3 then
			link_cst3.C.o = veclist[3].o
		end
		count = 1
		veclist = {}
		hit = true
		bshow = true
		dophg("script/test.txt")
	end
	if button1(650, 250, "clear") then
		count = 1
		veclist = {}
		hit = true
		bshow = false
	end
	if button1(650, 360, "exit") then
		exit()
	end
	return hit
end
function onmouse()
	local x,y,z = getvec3("mouse")
	pix(x, y)
	text(x, y - 20,'A')
	arrow(x,y,x,y + 50);
end