
class Tube
{
	constructor(gapY)
	{
		sprTopHead = luna.graphics.Sprite(luna.assets["art/items/tube_head"])
		sprTopBody = luna.graphics.Sprite(luna.assets["art/items/tube_body"])
		sprBottomHead = luna.graphics.Sprite(luna.assets["art/items/tube_head"])
		sprBottomBody = luna.graphics.Sprite(luna.assets["art/items/tube_body"])

		local halfGap = settings.gapSize / 2
		sprBottomHead.setY(gapY - halfGap - sprBottomHead.getHeight())
		sprBottomBody.setHeight(sprBottomHead.getY())

		sprTopHead.setScaleY(-1)
		sprTopHead.setY(gapY + halfGap)
		sprTopBody.setY(sprTopHead.getY())
		sprTopBody.setHeight(luna.sizes.getScreenHeight() - sprTopHead.getY() + sprTopHead.getHeight())

		setX(luna.sizes.getScreenWidth())

		local topRectY = sprTopHead.getY() - sprTopHead.getHeight()
		topRect = luna.math.Rect(0, topRectY, 
			sprTopHead.getWidth(),  luna.sizes.getScreenHeight() - topRectY)
		bottomRect = luna.math.Rect(0, 0, 
			sprTopHead.getWidth(),  sprBottomHead.getY() + sprBottomHead.getHeight())
	}

	sprTopHead = null
	sprTopBody = null
	sprBottomHead = null
	sprBottomBody = null
	topRect = null
	bottomRect = null

	function setX(x)
	{
		sprTopHead.setX(x)
		sprTopBody.setX(x)
		sprBottomHead.setX(x)
		sprBottomBody.setX(x)
	}

	function isOutOfScreen()
	{
		return sprTopHead.getX() + sprTopHead.getWidth() < 0
	}

	function isIntersectWithRect(rect)
	{
		topRect.x = sprTopHead.getX()
		bottomRect.x = sprBottomHead.getX()

		return luna.intersect.rectangles(topRect, rect) || luna.intersect.rectangles(bottomRect, rect) 
	}

	function update(dt)
	{
		setX(sprTopHead.getX() - settings.tubeVelocity * dt)
	}

	function render()
	{
		sprTopHead.render()
		sprTopBody.render()
		sprBottomHead.render()
		sprBottomBody.render()
	}
}