
Background = class()

function Background:onInit()
	self.sprites = self:makeSprites(luna.assets.art.items.bg)
	self.buildings = self:makeSprites(luna.assets.art.items.buildings)
	self.moon = luna.graphics.Sprite(luna.assets.art.items.moon)
	self.moon:setPos(luna.sizes.getScreenWidth() - 100, luna.sizes.getScreenHeight() - 100)
end

function Background:makeSprites(asset)
	local ret = {}

	for i = 1, 2 do
		local spr = luna.graphics.Sprite(asset)
		spr:setX(spr:getWidth() * (i - 1))
		table.insert(ret, spr)
	end	

	return ret
end

function Background:updateSprites(sprites, velocity, dt)
	for i, spr in pairs(sprites) do
		local dx = velocity * dt
		spr:setX(spr:getX() - dx)
		if spr:getX() + spr:getWidth() <= 0 then
			local anotherSpr = i == 1 and sprites[2] or sprites[1]
			spr:setX(anotherSpr:getX() + anotherSpr:getWidth() - dx)
		end
	end
end

function Background:update(dt)
	self:updateSprites(self.sprites, settings.bgVelocity, dt)
	self:updateSprites(self.buildings, settings.bgVelocity * settings.bgBuildingsMultipler, dt)
end

function Background:renderSprites(sprites)
	for _, spr in pairs(sprites) do
		spr:render()
	end
end

function Background:render()
	self:renderSprites(self.sprites)
	self:renderSprites(self.buildings)
	self.moon:render()
end