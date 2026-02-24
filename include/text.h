#pragma once

#define TEXT_MAX_BUFFERS			(16)
#define TEXT_FLAGS_BACKGROUND		(0 << 0)
#define TEXT_FLAGS_SPRITES			(1 << 0)
#define TEXT_FLAGS_SPRITES_HAVE_PRIO	(2 << 0)

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t length;
	uint8_t byte_count;
	union
	{
		struct
		{
			unsigned is_sprite_text : 1;
			unsigned sprites_have_priority : 1;
			unsigned unused : 6;
		};
		uint8_t flags;
	};
	uint16_t data[WS_DISPLAY_WIDTH_TILES];
} text_buffer;

void text_reset(ws_screen_t ws_iram* screen);
void text_print(uint8_t x, uint8_t y, uint8_t flags, const char __far * fmt, ...);
void text_flush(void);
