--[[
The update function is called every frame.In addition to this you can create your own global variables and functions.

	Usable Modules :
		You can use most default libraries.Calling print will print to the console that opens with the game.

		Game specific modules :

		snek:
			snek.setTurn(turnDirection)
				(IN)Number between - 1 and 1. - 1 Turns left and 1 turns right. 0 goes straight.
				(RETURNS)No return value

			snek.getObstacles(radius)
				(IN)Radius of search.The search is from the position of the snek which is its head.
				(RETURN)List of points.A point structure has an x, y position a radius rand a type.
				Current types :
					-snek

			snek.getPosition()
				(IN)Nothing
				(OUT) Position of snake head.Returned as two values so the syntax x, y = snek.getPosition() will work.

		screen :
			screen.getWidth()
				(IN)Nothing
				(OUT) Width of playable area

			screen.getHeight()
				(IN)Nothing
				(OUT) Height of playable area
--]]

function distance(x1, y1, x2, y2)
	return math.sqrt(math.pow(x2-x1,2)+math.pow(y2-y1,2))
end

function dot(x1, y1, x2, y2)
	return x1*x2 + y1*y2
end

function mag(x, y)
	return math.sqrt(math.pow(x,2) + math.pow(y,2))
end

function angleVectors(x1, y1, x2, y2)
	--print(x1.." "..y1.." "..x2.." "..y2)
	dotProduct = dot(x1, y1, x2, y2)
	--print("dot "..dotProduct)
	mag1 = mag(x1, y1)
	mag2 = mag(x2, y2)
	
	--print("mag1 "..mag1.." mag2 "..mag2)
	--print(cosTheta)
	cosTheta = dotProduct/(mag1*mag2)
	--print(math.atan2(x1*y2 - y1*x2, x1*x2+y1*y2))
	return math.atan2(x1*y2 - y1*x2, x1*x2+y1*y2)
end

function notNilPoint(point)
	if (point and point.x ~= nil and point.y ~= nil and point.r ~= nil and point.type ~= nil) then
		return true
	else
		return false
	end
end

function getWallTurnDirection(xZero, xMax, yZero, yMax, velX, velY)
	minDist = math.min(math.min(yZero, yMax), math.min(xMax, xZero))

	if (minDist > 150) then
		return 0,minDist
	end
	if (xMax-1 <= minDist) then
		if (dot(xMax,0,velX,velY) > 0) then
			if (angleVectors(-xMax, 0, velX, velY) < 0) then
				return 1,minDist
			else
				return -1,minDist
			end
		else
			return 0,minDist
		end
	elseif (xZero-1 <= minDist) then
		if (dot(-xZero,0,velX,velY) > 0) then
			if (angleVectors(xZero, 0, velX, velY) < 0) then
				return 1,minDist
			else
				return -1,minDist
			end
		else
			return 0,minDist
		end
	elseif (yMax-1 <= minDist) then
		if (dot(0,yMax,velX,velY) > 0) then
			if (angleVectors(0, -yMax, velX, velY) < 0) then
				return 1,minDist
			else
				return -1,minDist
			end
		else
			return 0,minDist
		end
	elseif (yZero-1 <= minDist) then
		if (dot(0,-yZero,velX,velY) > 0) then
			if (angleVectors(0, yZero, velX, velY) < 0) then
				return 1,minDist
			else
				return -1,minDist
			end
		else
			return 0,minDist
		end
	end
end
turnTurn = false
function update()
	x, y = snek.getPosition()
	velX, velY = snek.getVelocity()
	points = snek.getObstacles(100.0)
	width = screen.getWidth()
	height = screen.getHeight()

	shouldTurn = false
	angle = 0
	closestSnek = 10000
	if (points ~= nil) then
		if (notNilPoint(points[1])) then
			dist = distance(x, y, points[1].x, points[1].y)
			minIndex = 1
			for i, point in ipairs(points) do
				newDistance = distance(x, y, point.x, point.y)
				if (newDistance < dist and dot(point.x-x,point.y-y,velX,velY) > 0) then
					dist = newDistance
					minIndex = i
				end
			end
			closestSnek = dist
			closestPoint = points[minIndex]
			diffX = closestPoint.x - x
			diffY = closestPoint.y - y
			
			shouldTurn = dot(diffX, diffY, velX, velY) > 0
			if (shouldTurn) then
				angle = angleVectors(diffX, diffY, velX, velY)
			end
		end
	end
	wallTurn, wallDist = getWallTurnDirection(x, width-x, y, height-y, velX, velY)
	if (wallTurn ~= 0 and wallDist < closestSnek) then
		snek.setTurn(wallTurn)
	elseif (shouldTurn) then
		angle = angleVectors(diffX, diffY, velX, velY)
		if (angle > 0) then
			snek.setTurn(1)
		else
			snek.setTurn(-1)
		end
	elseif (turnTurn) then
		if (x > width/2) then
			snek.setTurn(1)
			turnTurn = false
		else
			snek.setTurn(-1)
			turnTurn = false
		end
	else
		snek.setTurn(0)
		turnTurn = true
	end
end