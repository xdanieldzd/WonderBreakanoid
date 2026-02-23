local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

local game_graphics = superfamiconv.convert_tileset(
	"game_graphics.png",
	superfamiconv.config()
		:mode("wsc"):bpp(4)
		:no_discard():no_flip()
)

process.emit_symbol("gfx_game_graphics", game_graphics)
process.emit_symbol("gfx_game_graphics_palette", game_graphics.palette)
