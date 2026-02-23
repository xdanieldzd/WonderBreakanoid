local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

local game_title = superfamiconv.convert_tilemap(
	"game_title.png",
	superfamiconv.config()
		:mode("wsc"):bpp(4)
		:tile_base(96):palette_base(8)
)

process.emit_symbol("gfx_game_title", game_title)
