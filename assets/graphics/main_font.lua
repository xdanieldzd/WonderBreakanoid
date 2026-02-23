local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

local main_font = superfamiconv.convert_tileset(
	"main_font.png",
	superfamiconv.config()
		:mode("wsc"):bpp(4)
		:no_discard():no_flip()
)

process.emit_symbol("gfx_main_font", main_font)
process.emit_symbol("gfx_main_font_palette", main_font.palette)
