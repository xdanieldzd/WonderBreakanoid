#include <stdlib.h>
#include <wonderful.h>
#include <ws.h>

#include "main.h"
#include "fade.h"

fade fade_data;

void fade_update(void)
{
	if (!fade_data.is_active) return;

	if (!fade_data.divider) fade_data.divider = 1;

	if ((vbl_ticks % (75 / fade_data.divider)) == 0)
	{
		switch (fade_data.state)
		{
			case FADE_STATE_OUT:
			{
				for (uint8_t i = 0; i < FADE_PALETTE_COUNT; i++)
					if (fade_data.ignored_palette_idx != i)
						fade_palette_out(i, fade_data.mode);

				fade_data.timer++;
				if (fade_data.timer >= (fade_data.until + 1))
				{
					fade_data.is_active = 0;
					fade_data.timer = 0;
				}
			}
			break;

			case FADE_STATE_IN:
			{
				for (uint8_t i = 0; i < FADE_PALETTE_COUNT; i++)
					if (fade_data.ignored_palette_idx != i)
						fade_palette_in(i, fade_data.mode);

				fade_data.timer++;
				if (fade_data.timer >= (fade_data.until + 1))
				{
					fade_data.state = FADE_STATE_CLEAR;
					fade_data.timer = 0;
				}
			}
			break;

			case FADE_STATE_CLEAR:
			{
				for (uint8_t i = 0; i < FADE_PALETTE_COUNT; i++)
					fade_palette_free(i);

				fade_data.is_active = 0;
				fade_data.timer = 0;
			}
			break;
		}
	}
}

void fade_palette_load(const uint8_t palette_idx, const __far uint8_t* palette_data)
{
	if (fade_data.palette_buffer[palette_idx] != NULL) fade_palette_free(palette_idx);

	fade_data.palette_buffer[palette_idx] = calloc(16, sizeof(uint16_t));
	memcpy(fade_data.palette_buffer[palette_idx], palette_data, 16 * sizeof(uint16_t));
}

void fade_palette_free(const uint8_t palette_idx)
{
	if (fade_data.palette_buffer[palette_idx] != NULL)
	{
		free(fade_data.palette_buffer[palette_idx]);
		fade_data.palette_buffer[palette_idx] = NULL;
	}
}

void fade_palette_out(const uint8_t palette_idx, const uint8_t mode)
{
	wscolor* ptr = (wscolor*)WS_DISPLAY_COLOR_MEM(palette_idx);
	for (uint16_t i = 0; i < 16; i++)
	{
		if (mode == 0)
		{
			if (ptr->r) ptr->r--;
			if (ptr->g) ptr->g--;
			if (ptr->b) ptr->b--;
		}
		else
		{
			if (ptr->r < 15) ptr->r++;
			if (ptr->g < 15) ptr->g++;
			if (ptr->b < 15) ptr->b++;
		}
		ptr++;
	}
}

void fade_palette_in(const uint8_t palette_idx, const uint8_t mode)
{
	if (fade_data.palette_buffer[palette_idx] == NULL) return;

	const wscolor* ptr_source = fade_data.palette_buffer[palette_idx];
	wscolor* ptr_dest = (wscolor*)WS_DISPLAY_COLOR_MEM(palette_idx);

	for (uint16_t i = 0; i < 16; i++)
	{
		if (mode == 0)
		{
			if (ptr_dest->r < ptr_source->r) ptr_dest->r++;
			if (ptr_dest->g < ptr_source->g) ptr_dest->g++;
			if (ptr_dest->b < ptr_source->b) ptr_dest->b++;
		}
		else
		{
			if (ptr_dest->r > ptr_source->r) ptr_dest->r--;
			if (ptr_dest->g > ptr_source->g) ptr_dest->g--;
			if (ptr_dest->b > ptr_source->b) ptr_dest->b--;
		}
		ptr_source++;
		ptr_dest++;
	}
}
