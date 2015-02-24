
Tube = class()

function Tube:onInit(gapY)
	self.sprTopHead = luna.graphics.Sprite(luna.assets.art.items.tube_head)
	self.sprTopBody = luna.graphics.Sprite(luna.assets.art.items.tube_body)
	self.sprBottomHead = luna.graphics.Sprite(luna.assets.art.items.tube_head)
	self.sprBottomBody = luna.graphics.Sprite(luna.assets.art.items.tube_body)	

	local halfGap = settings.gapSize / 2
	self.sprBottomHead:setY(gapY - halfGap - self.sprBottomHead:getHeight())
	self.sprBottomBody:setHeight(self.sprBottomHead:getY())

	self.sprTopHead:setScaleY(-1)
	self.sprTopHead:setY(gapY + halfGap)
	self.sprTopBody:setY(self.sprTopHead:getY())
	self.sprTopBody:setHeight(luna.sizes.getScreenHeight() - self.sprTopHead:getY() + self.sprTopHead:getHeight())

	self:setX(luna.sizes.getScreenWidth())
end

function Tube:setX(x)
	self.sprTopHead:setX(x)
	self.sprTopBody:setX(x)
	self.sprBottomHead:setX(x)
	self.sprBottomBody:setX(x)
end

function Tube:isOutOfScreen()
	return self.sprTopHead:getX() + self.sprTopHead:getWidth() < 0
end

function Tube:isIntersectWithRect(rect)
	local topRect = {}
	topRect.x = self.sprTopHead:getX()
	topRect.y = self.sprTopHead:getY() - self.sprTopHead:getHeight()
	topRect.width = self.sprTopHead:getWidth()
	topRect.height = luna.sizes.getScreenHeight() - topRect.y

	local bottomRect = {}
	bottomRect.x = self.sprBottomHead:getX()
	bottomRect.y = 0
	bottomRect.width = self.sprBottomHead:getWidth()
	bottomRect.height = self.sprBottomHead:getY() + self.sprBottomHead:getHeight()

	return luna.intersect.rectangles(topRect, rect) or luna.intersect.rectangles(bottomRect, rect) 
end

function Tube:update(dt)
	self:setX(self.sprTopHead:getX() - settings.tubeVelocity * dt)
end

function Tube:render()
	self.sprTopHead:render()
	self.sprTopBody:render()
	self.sprBottomHead:render()
	self.sprBottomBody:render()
end