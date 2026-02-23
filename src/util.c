#include <wonderful.h>
#include <ws.h>

void util_generate_sprite_tile_data(uint16_t* tile_data, const uint8_t count, const uint16_t first_tile_idx, const uint8_t palette_idx, const uint8_t has_priority)
{
	for (uint8_t i = 0; i < count; i++)
		tile_data[i] = (first_tile_idx + i) | WS_SPRITE_ATTR_PALETTE(palette_idx) | (has_priority ? WS_SPRITE_ATTR_PRIORITY : 0x0000);
}

void util_generate_background_tile_data(uint16_t* tile_data, const uint8_t count, const uint16_t first_tile_idx, const uint8_t palette_idx, const uint8_t tile_bank)
{
	for (uint8_t i = 0; i < count; i++)
		tile_data[i] = (first_tile_idx + i) | WS_SCREEN_ATTR_PALETTE(palette_idx) | WS_SCREEN_ATTR_BANK(tile_bank);
}
