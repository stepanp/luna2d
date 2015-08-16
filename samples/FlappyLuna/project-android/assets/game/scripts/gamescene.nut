
require("background")
require("bird")
require("tube")

class GameScene
{
	constructor()
	{
		bg = Background()
		bird = Bird()

		sprGameOver = luna.graphics.Sprite(luna.assets["art/items/gameover"])
		sprGameOver.setOriginToCenter()
		sprGameOver.setPos(luna.sizes.getScreenWidth() / 2, luna.sizes.getScreenHeight() / 2)

		tubes = []
	}

	bg = null
	bird = null
	tubes = null
	sprGameOver = null
	gameOver = false
	spawnTime = 0

	function spawnTube()
	{
		local gapY = luna.math.random(settings.gapMargins, luna.sizes.getScreenHeight() - settings.gapMargins)
		tubes.append(Tube(gapY))
	}

	function updateTubes(dt)
	{
		local count = tubes.len()
		local i = 0

		while(i < count)
		{
			local tube = tubes[i]
			tube.update(dt)

			if(tube.isOutOfScreen())
			{
				tubes.remove(i)
				count--
			}
			else i++
		}

		spawnTime -= dt

		if(spawnTime <= 0)
		{
			spawnTube()
			spawnTime = settings.tubeSpawnDelay
		}
	}

	function checkCollisions()
	{
		if(bird.isUnderFloor())
		{
			gameOver = true
			return
		}

		foreach(tube in tubes)
		{
			if(tube.isIntersectWithRect(bird.getRect()))
			{
				bird.fall()
				gameOver = true
				break
			}
		}
	}

	function onTouchDown(x, y, touchIndex)
	{
		if(touchIndex > 0) return;

		if(gameOver) 
		{
			luna.scenes.setScene(GameScene())
			return
		}

		bird.jump()
	}

	function onTouchMoved(x, y, touchIndex)
	{

	}

	function onTouchUp(x, y, touchIndex)
	{

	}

	function onUpdate(dt)
	{
		bird.update(dt)

		if(gameOver) return

		bg.update(dt)
		updateTubes(dt)
		checkCollisions()	
	}

	function onRender()
	{
		bg.render()
		bird.render()

		foreach(tube in tubes) tube.render()
		if(gameOver) sprGameOver.render()
	}
}