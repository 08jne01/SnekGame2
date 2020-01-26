--[[
The update function is called every frame.In addition to this you can create your own global variables and functions.

	Usable Modules :
		You can use most default libraries. Calling print will print to the console that opens with the game.

		Game specific modules :

		snek:
			snek.setTurn(turnDirection)
				(IN) Number between -1 and 1. -1 Turns left and 1 turns right. 0 goes straight.
				(RETURNS) No return value

			snek.getObstacles(radius)
				(IN) Radius of search.The search is from the position of the snek which is its head.
				(RETURN) List of points. A point structure has an x, y position a radius rand a type.
				Current types :
					-snek

			snek.getPosition()
				(IN) Nothing
				(OUT) Position of snake head. Returned as two values so the syntax x, y = snek.getPosition() will work.
			snek.getVelocity()
				(IN) Nothing
				(OUT) Velocity of snek. Returned as two values so the syntax velx, vely = snek.getVelocity() will work.

		screen :
			screen.getWidth()
				(IN) Nothing
				(OUT) Width of playable area

			screen.getHeight()
				(IN) Nothing
				(OUT) Height of playable area
--]]
--(IN) vector = x,y
--(RETURN) normalised vector
function normalise(x,y)
	mag = math.sqrt(math.pow(x,2) + math.pow(y,2))
	return x/mag, y/mag
end
--(IN) vector1 = x1,y1
--(IN) vector2 = x2,y2
--(RETURN) dot product of vectors
function dot(x1, y1, x2, y2)
	return x1*x2 + y1*y2
end
--(IN) point struct
--(RETURN) whether it or its member variables are nil
function notNilPoint(point)
	if (point and point.x ~= nil and point.y ~= nil and point.r ~= nil and point.type ~= nil) then
		return true
	else
		return false
	end
end

--(IN) vector1 = x1,y1
--(IN) vector2 = x2,y2
--(RETURN) distance between two points
function distance(x1, y1, x2, y2)
	return math.sqrt(math.pow(x2-x1,2)+math.pow(y2-y1,2))
end
--(IN) vector1 = x1,y1
--(IN) vector2 = x2,y2
--(RETURN) signed angle between them
function angleVectors(x1, y1, x2, y2)
	return math.atan2(x1*y2 - y1*x2, x1*x2+y1*y2)
end

function wallVector(x,y,width,height)
	minX = 1000.0/x --distance from left wall
	minY = 1000.0/y --distance from top wall
	maxX = 1000.0/(width-x) --distance from right wall
	maxY = 1000.0/(height-y) --distance from bottom wall

	xSteer = minX-maxX
	ySteer = minY-maxY
	magSqr = math.pow(xSteer,2)+math.pow(ySteer,2)
	print(magSqr)
	if (magSqr > 50) then
		return xSteer, ySteer
	else
		return false, false
	end
end

function minDist(points, x, y)
	dist = distance(x,y,points[1].x, points[1].y)
	for i, point in ipairs(points) do
		dist = math.min(dist, distance(x,y,point.x, point.y))
	end
	return dist
end

function getPoint(points, x, y)
	dist = distance(x,y,points[1].x, points[1].y)
	index = 1
	for i, point in ipairs(points) do
		d = distance(x,y,point.x, point.y)
		if (d < dist) then
			dist = d
			index = i
		end
	end
	return points[i]
end

function rayMarch(points, x, y, xVel, yVel)
	if (points and notNilPoint(points[1])) then
		r = 0
		px = x
		py = y
		rdx, rdy = normalise(xVel,yVel)
		while (r < 100) do
			step = minDist(points,x,y)
			if (step < 5) then
				break
			end
			px = px + rdx*step
			py = py + rdy*step
			r = r + step
		end

		

	else
		return 0
	end
end

function update()
	x,y = snek.getPosition()
	xV,yV = snek.getVelocity()
	width = screen.getWidth()
	height = screen.getHeight()
	points = snek.getObstacles(100.0)

	xSteer, ySteer = wallVector(x,y,width,height)
	if (xSteer and ySteer) then
		angle = angleVectors(xSteer,ySteer, xV,yV)
		--print(xSteer.." "..ySteer.." "..angle)
		if (angle > 0) then
			snek.setTurn(-1)
		else
			snek.setTurn(1)
		end
	else
		snek.setTurn(0)
	end
end