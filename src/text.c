#include <stdlib.h>
#include <stdio.h>
#include <wonderful.h>
#include <ws.h>
#include <wse.h>

#include "text.h"
#include "vec2.h"
#include "sprite.h"

ws_screen_t ws_iram* text_screen = NULL;

text_buffer text_buffers[TEXT_MAX_BUFFERS];
uint8_t num_used_text_buffers = 0;

void text_reset(ws_screen_t ws_iram* screen)
{
	text_screen = screen;
	num_used_text_buffers = 0;
}

void text_print(uint8_t x, uint8_t y, uint8_t flags, const char __far * fmt, ...)
{
	char buf[128];
	const char* ptr_source;
	uint16_t* ptr_dest;

	if (num_used_text_buffers < TEXT_MAX_BUFFERS)
	{
		va_list args;
		va_start(args, fmt);
		text_buffers[num_used_text_buffers].length = vsnprintf(buf, sizeof buf, fmt, args);
		va_end(args);

		if (text_buffers[num_used_text_buffers].length > WS_DISPLAY_WIDTH_TILES) text_buffers[num_used_text_buffers].length = WS_DISPLAY_WIDTH_TILES;
		memset(text_buffers[num_used_text_buffers].data, 0, WS_DISPLAY_WIDTH_TILES);

		text_buffers[num_used_text_buffers].byte_count = text_buffers[num_used_text_buffers].length * sizeof(uint16_t);

		text_buffers[num_used_text_buffers].x = x;
		text_buffers[num_used_text_buffers].y = y;
		text_buffers[num_used_text_buffers].flags = flags;

		for (ptr_source = buf, ptr_dest = text_buffers[num_used_text_buffers].data; *ptr_source != '\0';)
		{
			if (!text_buffers[num_used_text_buffers].is_sprite_text)
				*ptr_dest++ = (*ptr_source++ - 0x020) | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(15);
			else
				*ptr_dest++ = (*ptr_source++ - 0x020) | WS_SPRITE_ATTR_PALETTE(15) | WS_SPRITE_ATTR_PRIORITY;
		}

		num_used_text_buffers++;
	}
}

void text_flush(void)
{
	for (uint8_t i = 0; i < num_used_text_buffers; i++)
	{
		if (!text_buffers[i].is_sprite_text)
		{
			memcpy(&text_screen->row[((text_buffers[i].y & 0x1F))].cell[(text_buffers[i].x & 0x1F)], text_buffers[i].data, text_buffers[i].byte_count);
		}
		else
		{
			vec2 position = (struct vec2_s) { .x.high = text_buffers[i].x, .y.high = text_buffers[i].y };

			const uint16_t* sprite_data = text_buffers[i].data;
			for (uint8_t j = 0; j < text_buffers[i].length; j++)
			{
				if ((*sprite_data & WS_SPRITE_ATTR_TILE_MASK) != 0x000)
					sprite_set(&position, sprite_data);

				position.x.high += 8;
				sprite_data++;
			}
		}
	}
}
