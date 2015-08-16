
class Bird
{
	constructor()
	{
		spr = luna.graphics.Sprite(luna.assets["art/items/bird"])
		spr.setOriginToCenter()
		spr.setX(settings.birdPosX)
		spr.setY(luna.sizes.getScreenHeight() / 2)

		sprFlap = luna.graphics.Sprite(luna.assets["art/items/bird_flap"])
		sprFlap.setOriginToCenter()
		sprFlap.setX(spr.getX())
		sprFlap.setY(spr.getY())

		rect = luna.math.Rect(0, 0, settings.birdBoundsSize, settings.birdBoundsSize)
	}

	spr = null
	sprFlap = null
	rect = null
	curVelocity = 0
	curAngle = 0
	curAngleVelocity = 0
	flapTime = 0

	function isUnderFloor()
	{
		return spr.getY() + spr.getHeight() / 2 < 0
	}

	function jump()
	{
		curVelocity = settings.birdJumpVelocity
		curAngle = settings.birdMinAngle
		curAngleVelocity = 0
		flapTime = settings.birdFlapTime
	}

	function fall()
	{
		curVelocity = 0
	}

	function getRect()
	{
		rect.x = spr.getX() - settings.birdBoundsSize / 2
		rect.y = spr.getY() - settings.birdBoundsSize / 2

		return rect
	}

	function update(dt)
	{
		curVelocity = curVelocity - settings.gravity * dt
		local y = spr.getY() + curVelocity

		if(y >= luna.sizes.getScreenHeight())
		{
			y = luna.sizes.getScreenHeight()
			curVelocity = 0
			flapTime = 0
		}

		spr.setY(y)
		sprFlap.setY(y)

		curAngleVelocity = curAngleVelocity - settings.birdAngleVelocity * dt
		curAngle = luna.math.max(curAngle + curAngleVelocity, settings.birdMaxAngle)
		spr.setAngle(curAngle)

		if(flapTime > 0) flapTime = flapTime - dt
	}

	function render()
	{
		if(flapTime > 0) sprFlap.render()
		else spr.render()
	}
}