
-- This file provides backward compability with version 0.5

local _lines = luna.intersect.lines

luna.intersect.lines = function(line1, line2)
	return _lines(math.Vec2(line1.x1, line1.y1), math.Vec2(line1.x2, line1.y2), 
		math.Vec2(line2.x1, line2.y1), math.Vec2(line2.x2, line2.y2))
end

local _pointBetweenLines = luna.intersect.pointBetweenLines

luna.intersect.pointBetweenLines = function(line1, line2)
	return _pointBetweenLines(math.Vec2(line1.x1, line1.y1), math.Vec2(line1.x2, line1.y2), 
		math.Vec2(line2.x1, line2.y1), math.Vec2(line2.x2, line2.y2))
end
