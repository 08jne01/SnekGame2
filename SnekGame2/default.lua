--[[
The update function is called every frame. In addition to this you can create your own global variables and functions.

Usable Modules:
	You can use most default libraries. Calling print will print to the console that opens with the game.

	Game specific modules:

	snek:
		snek.setTurn(turnDirection)
			(IN) Number between -1 and 1. -1 Turns left and 1 turns right. 0 goes straight.
			(RETURNS) No return value

		snek.getObstacles(radius)
			(IN) Radius of search. The search is from the position of the snek which is its head.
			(RETURN) List of points. A point structure has an x, y position a radius r and a type.
			Current types:
				- snek

		snek.getPosition()
			(IN) Nothing
			(OUT) Position of snake head. Returned as two values so the syntax x, y = snek.getPosition() will work.

	screen:
		screen.getWidth()
			(IN) Nothing
			(OUT) Width of playable area

		screen.getHeight()
			(IN) Nothing
			(OUT) Height of playable area
--]]

function update()
	
end