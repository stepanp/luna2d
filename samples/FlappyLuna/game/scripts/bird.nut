
class Bird
{
	constructor()
	{
		spr = luna.graphics.Sprite(luna.assets["art/items/bird"])
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