#pragma once

void util_generate_sprite_tile_data(uint16_t* tile_data, const uint8_t count, const uint16_t first_tile_idx, const uint8_t palette_idx, const uint8_t has_priority);
void util_generate_background_tile_data(uint16_t* tile_data, const uint8_t count, const uint16_t first_tile_idx, const uint8_t palette_idx, const uint8_t tile_bank);
