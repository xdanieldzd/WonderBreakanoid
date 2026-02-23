local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

local game_background_3 = superfamiconv.convert_tilemap(
	"game_background_3.png",
	superfamiconv.config()
		:mode("wsc"):bpp(4)
		:tile_base(256):palette_base(0)
)

process.emit_symbol("gfx_game_background_3", game_background_3)
