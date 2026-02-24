#include <wonderful.h>
#include <ws.h>
#include <wse.h>

#include "vec2.h"

ws_sprite_table_t* sprite_table = NULL;

uint8_t sprite_count = 0;

void sprite_reset(ws_sprite_table_t* table)
{
	sprite_table = table;
	memset(sprite_table, 0, sizeof(ws_sprite_t) * sprite_count);
	sprite_count = 0;
}

void sprite_set(const vec2* position, const uint16_t* tile_data)
{
	sprite_table->entry[sprite_count].x = position->x.high;
	sprite_table->entry[sprite_count].y = position->y.high;
	sprite_table->entry[sprite_count].attr = *tile_data;
	sprite_count++;
}

void sprite_set_multi(const vec2* position, const vec2 pos_increment, const uint16_t* tile_data, const uint8_t count)
{
	vec2 next_position = *position;

	for (uint8_t i = 0; i < count; i++)
	{
		sprite_table->entry[sprite_count].x = next_position.x.high;
		sprite_table->entry[sprite_count].y = next_position.y.high;
		sprite_table->entry[sprite_count].attr = *tile_data;

		vec2_add(&next_position, &pos_increment, &next_position);

		sprite_count++;
		tile_data++;
	}
}

void sprite_insert_first(const vec2* position, const uint16_t* tile_data)
{
	for (uint8_t i = sprite_count; i > 0; i--)
		sprite_table->entry[i] = sprite_table->entry[i - 1];

	sprite_table->entry[0].x = position->x.high;
	sprite_table->entry[0].y = position->y.high;
	sprite_table->entry[0].attr = *tile_data;
	sprite_count++;
}

void sprite_flush(void)
{
	outportb(WS_SPR_COUNT_PORT, sprite_count);
}
