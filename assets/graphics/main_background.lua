local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

local main_background = superfamiconv.convert_tilemap(
	"main_background.png",
	superfamiconv.config()
		:mode("wsc"):bpp(4)
		:tile_base(256):palette_base(0)
)

process.emit_symbol("gfx_main_background", main_background)
