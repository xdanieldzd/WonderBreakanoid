#pragma once

#include "wscolor.h"

#define FADE_MODE_BLACK				(0 << 0)
#define FADE_MODE_WHITE				(1 << 0)

#define FADE_PALETTE_COUNT			(16)

enum FADE_STATE
{
	FADE_STATE_OUT = 0,
	FADE_STATE_IN,
	FADE_STATE_CLEAR,
	FADE_STATE_INVALID = -1
};

typedef struct
{
	wscolor* palette_buffer[16];
	uint16_t timer;
	uint8_t divider;
	uint8_t until;
	uint8_t ignored_palette_idx;
	union
	{
		struct
		{
			unsigned is_active : 1;
			unsigned state : 3;
			unsigned mode : 1;
			unsigned unused : 3;
		};
		uint8_t flags;
	};
} fade;

extern fade fade_data;

void fade_update(void);
void fade_palette_load(const uint8_t palette_idx, const __far uint8_t* palette_data);
void fade_palette_free(const uint8_t palette_idx);
void fade_palette_out(const uint8_t palette_idx, const uint8_t mode);
void fade_palette_in(const uint8_t palette_idx, const uint8_t mode);
