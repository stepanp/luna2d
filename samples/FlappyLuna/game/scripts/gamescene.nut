
require("background")
require("bird")

class GameScene
{
	constructor()
	{
		bg = Background()
		bird = Bird()

		sprGameOver = luna.graphics.Sprite(luna.assets["art/items/gameover"])
		sprGameOver.setOriginToCenter()
		sprGameOver.setPos(luna.sizes.getScreenWidth() / 2, luna.sizes.getScreenHeight() / 2)
	}

	bg = null
	bird = null
	sprGameOver = null
	gameOver = false

	function onTouchDown(x, y, touchIndex)
	{
		if(touchIndex > 0) return;

		if(gameOver) 
		{
			luna.scenes.setScene(GameScene())
			return;
		}

	}

	function onTouchMoved(x, y, touchIndex)
	{

	}

	function onTouchUp(x, y, touchIndex)
	{

	}

	function onUpdate(dt)
	{
	}

	function onRender()
	{
		bg.render()
		bird.render()

		if(gameOver) sprGameOver.render()
	}
}