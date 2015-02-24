
Bird = class()

function Bird:onInit()
	self.spr = luna.graphics.Sprite(luna.assets.art.items.bird)
	self.spr:setOriginToCenter()
	self.spr:setX(settings.birdPosX)
	self.spr:setY(luna.sizes.getScreenHeight() / 2)

	self.sprFlap = luna.graphics.Sprite(luna.assets.art.items.bird_flap)
	self.sprFlap:setOriginToCenter()
	self.sprFlap:setX(self.spr:getX())
	self.sprFlap:setY(self.spr:getY())	

	self.curVelocity = 0
	self.curAngle = 0
	self.curAngleVelocity = 0
	self.flapTime = 0
end

function Bird:isUnderFloor()
	return self.spr:getY() + self.spr:getHeight() / 2 < 0
end

function Bird:jump()
	self.curVelocity = settings.birdJumpVelocity
	self.curAngle = settings.birdMinAngle
	self.curAngleVelocity = 0
	self.flapTime = settings.birdFlapTime
end

function Bird:fall()
	self.curVelocity = 0
end

function Bird:getRect()
	local rect = {}

	rect.x = self.spr:getX() - settings.birdBoundsSize / 2
	rect.y = self.spr:getY() - settings.birdBoundsSize / 2
	rect.width = settings.birdBoundsSize
	rect.height = settings.birdBoundsSize

	return rect
end

function Bird:update(dt)
	self.curVelocity = self.curVelocity - settings.gravity * dt
	local y = self.spr:getY() + self.curVelocity

	if y >= luna.sizes:getScreenHeight() then
		y = luna.sizes:getScreenHeight()
		self.curVelocity = 0
		self.flapTime = 0
	end

	self.spr:setY(y)
	self.sprFlap:setY(y)

	self.curAngleVelocity = self.curAngleVelocity - settings.birdAngleVelocity * dt
	self.curAngle = math.max(self.curAngle + self.curAngleVelocity, settings.birdMaxAngle)
	self.spr:setAngle(self.curAngle)

	if self.flapTime > 0 then
		self.flapTime = self.flapTime - dt
	end
end

function Bird:render()
	if self.flapTime > 0 then
		self.sprFlap:render()
	else
		self.spr:render()
	end
end