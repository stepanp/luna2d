
class Background
{
	constructor()
	{
		spr = luna.graphics.Sprite(luna.assets["art/items/bg"])
	}

	spr = null

	function update(dt)
	{
	}

	function render()
	{
		spr.render()
	}
}