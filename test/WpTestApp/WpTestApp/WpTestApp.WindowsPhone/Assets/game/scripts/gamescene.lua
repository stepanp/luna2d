
require("background")
require("bird")
require("tube")

GameScene = class()

function GameScene:onInit()
	self.bird = Bird()
	self.bg = Background()
	self.tubes = {}

	self.gameOverImg = luna.graphics.Sprite(luna.assets.art.items.gameover)
	self.gameOverImg:setOriginToCenter()
	self.gameOverImg:setPos(luna.sizes.getScreenWidth() / 2, luna.sizes.getScreenHeight() / 2)

	self.spawnTime = 0
	self.gameOver = false
end

function GameScene:spawnTube()
	local gapY = math.random(settings.gapMargins, luna.sizes.getScreenHeight() - settings.gapMargins)
	table.insert(self.tubes, Tube(gapY))
end

function GameScene:updateTubes(dt)
	local count = #self.tubes
	local i = 1
	while i <= count do
		local tube = self.tubes[i]
		tube:update(dt)

		if tube:isOutOfScreen() then
			table.remove(self.tubes, i)
			count = #self.tubes
		else
			i = i + 1
		end
	end

	self.spawnTime = self.spawnTime - dt

	if self.spawnTime <= 0 then
		self:spawnTube()
		self.spawnTime = settings.tubeSpawnDelay
	end
end

function GameScene:checkCollisions()
	if self.bird:isUnderFloor() then
		self.gameOver = true
		return
	end

	for _, tube in pairs(self.tubes) do
		if tube:isIntersectWithRect(self.bird:getRect()) then
			self.bird:fall()
			self.gameOver = true
			break
		end
	end
end

function GameScene:onTouchDown(x, y)
	if self.gameOver then
		luna.scenes.setScene(GameScene())
	end

	self.bird:jump()
end

function GameScene:onTouchMoved(x, y)

end

function GameScene:onTouchUp(x, y)

end

function GameScene:onUpdate(dt)
	self.bird:update(dt)

	if self.gameOver then
		return
	end

	self.bg:update(dt)

	self:updateTubes(dt)
	self:checkCollisions()
end

function GameScene:onRender()
	self.bg:render()
	self.bird:render()

	for _, tube in pairs(self.tubes) do
		tube:render()
	end

	if self.gameOver then
		self.gameOverImg:render()
	end
end