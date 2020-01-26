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
	if (magSqr > 1000) then
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
		if (d < dist and dot(x,y,point.x,point.y) > 0) then
			dist = d
			index = i
		end
	end
	return points[index]
end
bias = -1
count = 0
behaviourCount = 0
spiral = true
function rayMarch(points, x, y, xVel, yVel)
	if (points and notNilPoint(points[1])) then
		r = 0
		px = x
		py = y
		rdx, rdy = normalise(xVel,yVel)
		while (r < 100) do
			step = minDist(points,px,py)
			px = px + rdx*step
			py = py + rdy*step
			r = r + step
			if (step < 10) then
				--print("Collision")
				break
			end
		end

		if (r < 100) then
			target = getPoint(points, x, y)
			--print(target.x.." "..target.y)
			if (target) then
				--print("also "..target.x.." "..target.y)
				diffX = target.x - x
				diffY = target.y - y
				return diffX, diffY
			else
				return false,false
			end
		else
			return false,false
		end
	else
		return false,false
	end
end

function update()
	count = count + 1
	behaviourCount = behaviourCount + 1
	if (behaviourCount > 600 or (behaviourCount > 300 and spiral == false)) then
		behaviourCount = 0
		if (spiral) then
			bias = -bias
			spiral = false
		else
			spiral = true
		end
	end
	if (spiral ~= true) then
		if (count > 120) then
			count = 0
			bias = -bias
		end
	end

	x,y = snek.getPosition()
	xV,yV = snek.getVelocity()
	width = screen.getWidth()
	height = screen.getHeight()
	points = snek.getObstacles(200.0)

	xSteer, ySteer = wallVector(x,y,width,height)
	turn = 0
	if (count % 20 == 0 or spiral) then
		turn = bias
	end
	biasOn = true
	if (xSteer and ySteer) then
		angle = angleVectors(xSteer,ySteer, xV,yV)
		turn = 0
		biasOn = false
		if (angle > 0) then
			turn = turn - 1
		else
			turn = turn + 1
		end
	end

	diffX, diffY = rayMarch(points, x, y, xV, yV)
	if (diffX and diffY) then
		angle = angleVectors(diffX, diffY, xV, yV)
		if (biasOn) then
			turn = 0
			biasOn = false
		end
		if (angle > 0) then
			turn = turn + 1
		else
			turn = turn - 1
		end
	end

	snek.setTurn(turn)
end