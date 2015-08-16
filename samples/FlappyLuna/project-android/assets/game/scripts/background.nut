
class Background
{
	constructor()
	{
		sprites = makeSprites(luna.assets["art/items/bg"])
		buildings = makeSprites(luna.assets["art/items/buildings"])
		moon = luna.graphics.Sprite(luna.assets["art/items/moon"])
		moon.setPos(luna.sizes.getScreenWidth() - 100, luna.sizes.getScreenHeight() - 100)
	}

	sprites = null
	buildings = null
	moon = null

	function makeSprites(asset)
	{
		local ret = []

		for(local i = 0; i < 2; i++)
		{
			local spr = luna.graphics.Sprite(asset)
			spr.setX(spr.getWidth() * i)
			ret.append(spr)
		}	

		return ret
	}

	function updateSprites(sprites, velocity, dt)
	{
		foreach(i, spr in sprites)
		{
			local dx = velocity * dt
			spr.setX(spr.getX() - dx)
			if(spr.getX() + spr.getWidth() <= 0)
			{
				local anotherSpr = (i == 0 ? sprites[1] : sprites[0])
				spr.setX(anotherSpr.getX() + anotherSpr.getWidth() - dx)
			}
		}
	}

	function update(dt)
	{
		updateSprites(sprites, settings.bgVelocity, dt)
		updateSprites(buildings, settings.bgVelocity * settings.bgBuildingsMultipler, dt)
	}

	function renderSprites(sprites)
	{
		foreach(spr in sprites) spr.render()
	}

	function render()
	{
		renderSprites(sprites)
		renderSprites(buildings)
		moon.render()
	}
}