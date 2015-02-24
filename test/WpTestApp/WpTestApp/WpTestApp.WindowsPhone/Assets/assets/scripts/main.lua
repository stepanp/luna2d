require("settings")
require("gamescene")

function luna.main()
	luna.assetsmgr.loadAll()
	luna.scenes.setScene(GameScene())
end