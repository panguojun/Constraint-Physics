----------------------------------------------
-- APP
----------------------------------------------
require ("script/cst")
----------------------------------------------
gui = 0
stage = 0
mousedownx = 0
mousedowny = 0
----------------------------------------------
function onlbtn()
	if onGUI() then
		return
	end
end
----------------------------------------------
function onlbtnup()
end
----------------------------------------------
-- onGUI
----------------------------------------------
function onGUI()
	ret = false
	rgb(158,158,158)
	cls()
	if(button_big(300, 50, "Chapter1")) then
		dolua ("script/chapter1.lua")
		cls()
		ret = true
	end
	if(button_big(300, 150, "Chapter2")) then
		dolua ("script/chapter2.lua")
		cls()
		ret = true
	end
	if(button_big(300, 300, "exit")) then
		gui = 2
		ret = true
		exit()
	end
	return ret
end
----------------------------------------------
-- onmouse
----------------------------------------------
function onmouse()
end
onGUI()