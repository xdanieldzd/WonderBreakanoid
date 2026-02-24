#pragma once

void sprite_reset(ws_sprite_table_t* table);
void sprite_set(const vec2* position, const uint16_t* tile_data);
void sprite_set_multi(const vec2* position, const vec2 pos_increment, const uint16_t* tile_data, const uint8_t count);
void sprite_insert_first(const vec2* position, const uint16_t* tile_data);
void sprite_flush(void);
