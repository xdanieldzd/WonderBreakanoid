#include <stdlib.h>
#include <wonderful.h>
#include <ws.h>
#include <wse.h>

#include "game.h"
#include "game_states.h"
#include "game_strings.h"
#include "game_graphics.h"
#include "game_levels.h"

#include "main.h"
#include "util.h"
#include "fade.h"
#include "sprite.h"
#include "text.h"
#include "sram.h"

game* game_work = NULL;

const uint8_t __far* level_data[6] = { game_level_data_1, game_level_data_2, game_level_data_3, game_level_data_4, game_level_data_5, game_level_data_6 };

const uint8_t brick_score_values[5] = { 0, 1, 2, 3, 5 };

void game_init(void)
{
	game_work = calloc(sizeof(game), 1);
	game_work->random_seed = SRAM_CHECKSUM_SEED;

	game_work->current_state = GAME_STATE_STARTUP;
	game_work->debug_enable = 0;

	ws_int_set_handler(WS_INT_VBLANK, game_vblank_int_handler);
	ws_int_set_handler(WS_INT_LINE_MATCH, game_line_match_int_handler);
}

void game_main(void)
{
	fade_update();

	if (fade_data.is_active) return;

	if (game_main_functions[game_work->current_state] != NULL)
		(*game_main_functions[game_work->current_state])();
}

void game_exit(void)
{
	sram_free();

	free(game_work);
	game_work = NULL;
}

__attribute__((assume_ss_data, interrupt)) void __far game_vblank_int_handler(void)
{
	vbl_ticks++;

	game_work->buttons.held = ws_keypad_scan();
	game_work->buttons.pressed = (game_work->buttons.held ^ game_work->buttons.held_last) & game_work->buttons.held;
	game_work->buttons.held_last = game_work->buttons.held;

	game_work->random_seed -= ((vbl_ticks + game_work->buttons.held) << 2) | ((game_work->random_seed >> 2) & 0x03);
	srand(game_work->random_seed);

	if (game_work->timer) game_work->timer--;

	if (game_work->do_process_text_sprites)
	{
		text_reset(&wse_screen2);
		sprite_reset(&wse_sprites1);
	}

	game_draw();

	if (game_work->do_process_text_sprites)
	{
		text_flush();
		sprite_flush();
	}

	if (game_work->current_state != game_work->next_state)
		game_work->current_state = game_work->next_state;

	ws_int_ack(WS_INT_ACK_VBLANK);
	ia16_enable_irq();
}

__attribute__((assume_ss_data, interrupt)) void __far game_line_match_int_handler(void)
{
	//

	ws_int_ack(WS_INT_ACK_LINE_MATCH);
	ia16_enable_irq();
}

void game_fade_in_from(const uint8_t mode, const uint8_t divider, const uint8_t until, const uint8_t* ignored_pal)
{
	fade_palette_load(15, gfx_main_font_palette);

	if (game_work->graphics == 0)
		fade_palette_load(8, gfx_game_graphics_palette);
	else if (game_work->graphics == 1)
		fade_palette_load(8, gfx_game_title_palette);

	if (game_work->background == 0)
		fade_palette_load(0, gfx_game_background_1_palette);
	else if (game_work->background == 1)
		fade_palette_load(0, gfx_game_background_2_palette);
	else if (game_work->background == 2)
		fade_palette_load(0, gfx_game_background_3_palette);

	fade_data.state = FADE_STATE_IN;
	fade_data.mode = mode;
	fade_data.timer = 0;
	fade_data.divider = divider;
	fade_data.until = until;
	fade_data.ignored_palette_idx = ignored_pal != NULL ? *ignored_pal : 0xFF;
	fade_data.is_active = 1;
}

void game_fade_out_to(const uint8_t mode, const uint8_t divider, const uint8_t until, const uint8_t* ignored_pal)
{
	fade_data.state = FADE_STATE_OUT;
	fade_data.mode = mode;
	fade_data.timer = 0;
	fade_data.divider = divider;
	fade_data.until = until;
	fade_data.ignored_palette_idx = ignored_pal != NULL ? *ignored_pal : 0xFF;
	fade_data.is_active = 1;
}

void game_load_graphics(uint8_t graphics)
{
	if (graphics == 0)
	{
		ws_gdma_copy(WS_TILE_4BPP_MEM(0x060), gfx_game_graphics_tiles, gfx_game_graphics_tiles_size);
	}
	else if (graphics == 1)
	{
		ws_gdma_copy(WS_TILE_4BPP_MEM(0x060), gfx_game_title_tiles, gfx_game_title_tiles_size);
		ws_gdma_copy(&wse_screen2, gfx_game_title_map, gfx_game_title_map_size);
	}
}

void game_load_background(uint8_t background)
{
	if (background == 0)
	{
		ws_gdma_copy(WS_TILE_4BPP_MEM(0x100), gfx_game_background_1_tiles, gfx_game_background_1_tiles_size);
		ws_gdma_copy(&wse_screen1, gfx_game_background_1_map, gfx_game_background_1_map_size);
	}
	else if (background == 1)
	{
		ws_gdma_copy(WS_TILE_4BPP_MEM(0x100), gfx_game_background_2_tiles, gfx_game_background_2_tiles_size);
		ws_gdma_copy(&wse_screen1, gfx_game_background_2_map, gfx_game_background_2_map_size);
	}
	else if (background == 2)
	{
		ws_gdma_copy(WS_TILE_4BPP_MEM(0x100), gfx_game_background_3_tiles, gfx_game_background_3_tiles_size);
		ws_gdma_copy(&wse_screen1, gfx_game_background_3_map, gfx_game_background_3_map_size);
	}
}

inline static void game_draw_border_line_large_horizontal(uint8_t x, uint8_t y, uint8_t width)
{
	for (uint8_t i = 0; x < width; x++, i++)
	{
		uint16_t tile_offset = i == 0 || i == width - 1 ? 0 : ((i % 2) == 0) ? 2 : 1;
		ws_screen_put_tile(&wse_screen2, (GAME_BORDER_FIRST_TILE_INDEX + tile_offset) | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), x, y);
	}
}

inline static void game_draw_border_line_large_vertical(uint8_t x, uint8_t y, uint8_t height)
{
	for (uint8_t i = 0; y < height; y++, i++)
	{
		uint16_t tile_offset = i == 0 || i == height - 1 ? 0 : ((i % 2) == 0) ? 4 : 3;
		ws_screen_put_tile(&wse_screen2, (GAME_BORDER_FIRST_TILE_INDEX + tile_offset) | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), x, y);
	}
}

void game_draw_border_lines(uint8_t data)
{
	if (data & GAME_BORDER_DRAW_LARGE)
	{
		if (data & GAME_BORDER_DRAW_TOP)		game_draw_border_line_large_horizontal(0, 0,						WS_DISPLAY_WIDTH_TILES);
		if (data & GAME_BORDER_DRAW_BOTTOM)	game_draw_border_line_large_horizontal(0, WS_DISPLAY_HEIGHT_TILES - 1,	WS_DISPLAY_WIDTH_TILES);
		if (data & GAME_BORDER_DRAW_LEFT) 	game_draw_border_line_large_vertical(0, 0,							WS_DISPLAY_HEIGHT_TILES);
		if (data & GAME_BORDER_DRAW_RIGHT)	game_draw_border_line_large_vertical(WS_DISPLAY_WIDTH_TILES - 1, 0,	WS_DISPLAY_HEIGHT_TILES);
	}
	else
	{
		if (data & GAME_BORDER_DRAW_TOP)		ws_screen_fill_tiles(&wse_screen2, GAME_BORDER_FIRST_TILE_INDEX | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), 0, 0,							WS_DISPLAY_WIDTH_TILES, 1);
		if (data & GAME_BORDER_DRAW_BOTTOM)	ws_screen_fill_tiles(&wse_screen2, GAME_BORDER_FIRST_TILE_INDEX | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), 0, WS_DISPLAY_HEIGHT_TILES - 1,	WS_DISPLAY_WIDTH_TILES, 1);
		if (data & GAME_BORDER_DRAW_LEFT) 	ws_screen_fill_tiles(&wse_screen2, GAME_BORDER_FIRST_TILE_INDEX | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), 0, 0,							1, WS_DISPLAY_HEIGHT_TILES);
		if (data & GAME_BORDER_DRAW_RIGHT)	ws_screen_fill_tiles(&wse_screen2, GAME_BORDER_FIRST_TILE_INDEX | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(8), WS_DISPLAY_WIDTH_TILES - 1, 0,	1, WS_DISPLAY_HEIGHT_TILES);
	}
}

void game_clear_screen_with_border(void)
{
	ws_screen_fill_tiles(&wse_screen2, 0x000 | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(0), 0, 0, WS_SCREEN_WIDTH_TILES, WS_SCREEN_HEIGHT_TILES);
	game_draw_border_lines(GAME_BORDER_DRAW_TOP | GAME_BORDER_DRAW_BOTTOM | GAME_BORDER_DRAW_LEFT | GAME_BORDER_DRAW_RIGHT | GAME_BORDER_DRAW_LARGE);
}

void game_clear_field(void)
{
	ws_screen_fill_tiles(&wse_screen2, 0x000 | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(0), 0, 0, WS_SCREEN_WIDTH_TILES, WS_SCREEN_HEIGHT_TILES);
	game_draw_border_lines(GAME_BORDER_DRAW_TOP | GAME_BORDER_DRAW_LEFT | GAME_BORDER_DRAW_RIGHT | GAME_BORDER_DRAW_LARGE);
}

void game_clear_results(void)
{
	game_work->score = 0;
	game_work->level = 0;
	game_work->lives = 2;
}

void game_begin()
{
	memset(game_work->ball, 0, sizeof(ball) * GAME_BALLS_MAX_COUNT);
	game_work->has_round_ended = 0;
	game_work->player_has_won = 0;

	game_spawn_paddle();
	game_spawn_ball((struct vec2_s) { .x.high = 108, .y.high = game_work->paddle.position.y.high - GAME_BALL_HEIGHT + 1 });
	game_update_bricks();
}

void game_spawn_paddle(void)
{
	game_work->paddle.position = (struct vec2_s) { .x.high = GAME_PADDLE_SPAWN_X_POSITION, .y.high = GAME_PADDLE_SPAWN_Y_POSITION };
	util_generate_sprite_tile_data(game_work->paddle.tile_data, GAME_PADDLE_TILE_COUNT, GAME_PADDLE_FIRST_TILE_INDEX, GAME_PADDLE_PALETTE_INDEX, 0);
	game_work->paddle.is_alive = 1;
	game_work->paddle.is_sticky = 1;
}

void game_spawn_ball(vec2 position)
{
	for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
	{
		if (game_work->ball[i].is_alive) continue;

		memset(&game_work->ball[i], 0, sizeof(ball));
		game_work->ball[i].position = position;
		game_work->ball[i].velocity = (struct vec2_s) { .x.low = 64, .y.high = 1 };
		util_generate_sprite_tile_data(game_work->ball[i].tile_data, GAME_BALL_TILE_COUNT, GAME_BALL_FIRST_TILE_INDEX, GAME_BALL_PALETTE_INDEX, 0);
		game_work->ball[i].is_alive = 1;

		return;
	}
}

void game_spawn_bricks(const __far uint8_t* bricks)
{
	uint8_t i = 0;

	for (uint8_t y = 0; y < GAME_BRICKS_FIELD_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < GAME_BRICKS_FIELD_WIDTH; x++)
		{
			game_work->bricks[i].position = (struct vec2_s) { .x.high = (x << 1) + 1, .y.high = y + 2 };
			game_work->bricks[i].type = *bricks;

			if ((game_work->bricks[i].type & GAME_BRICK_FLAGS_COLOR_MASK) != GAME_BRICK_FLAGS_COLOR_NONE)
			{
				game_work->bricks[i].is_alive = 1;

				util_generate_background_tile_data(
					game_work->bricks[i].tile_data,
					GAME_BRICK_TILE_COUNT,
					(GAME_BRICK_FIRST_TILE_INDEX + (((game_work->bricks[i].type & GAME_BRICK_FLAGS_COLOR_MASK) - 1) << 1)),
					GAME_BRICK_PALETTE_INDEX,
					0);
			}

			bricks++;
			i++;
		}
	}
}

void game_update_bricks(void)
{
	for (uint8_t i = 0; i < GAME_BRICKS_MAX_COUNT; i++)
	{
		if (*(uint32_t*)&game_work->bricks[i] == 0) continue;

		if (game_work->bricks[i].is_alive)
			ws_screen_put_tiles(&wse_screen2, &game_work->bricks[i].tile_data, game_work->bricks[i].position.x.high, game_work->bricks[i].position.y.high, ARRAY_LENGTH(game_work->bricks[i].tile_data), 1);
		else
			ws_screen_fill_tiles(&wse_screen2, 0x000 | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(0), game_work->bricks[i].position.x.high, game_work->bricks[i].position.y.high, ARRAY_LENGTH(game_work->bricks[i].tile_data), 1);
	}
}

void game_move_ball(const uint8_t index)
{
	if (game_work->ball[index].is_on_paddle) return;

	//if (!game_work->debug_enable)
	{
		if (game_work->ball[index].velocity.x.value >= GAME_BALL_MAX_VELOCITY) game_work->ball[index].velocity.x.value = GAME_BALL_MAX_VELOCITY;
		if (game_work->ball[index].velocity.y.value >= GAME_BALL_MAX_VELOCITY) game_work->ball[index].velocity.y.value = GAME_BALL_MAX_VELOCITY;
		if (game_work->ball[index].velocity.y.value > 0 && game_work->ball[index].velocity.x.value == 0) game_work->ball[index].velocity.x.low += 0x20;

		vec2_add(&game_work->ball[index].position, &game_work->ball[index].velocity, &game_work->ball[index].position);

		if (game_work->ball[index].position.x.high >= GAME_PLAYFIELD_MAX_X_POSITION) game_work->ball[index].velocity.x.value = -abs(game_work->ball[index].velocity.x.value);
		if (game_work->ball[index].position.x.high <= GAME_PLAYFIELD_MIN_X_POSITION) game_work->ball[index].velocity.x.value = abs(game_work->ball[index].velocity.x.value);

		if (game_work->ball[index].position.y.high <= GAME_PLAYFIELD_MIN_Y_POSITION) game_work->ball[index].velocity.y.value = -game_work->ball[index].velocity.y.value;
	}
	/*else
	{
		if (game_work->buttons.held & WS_KEY_Y1) game_work->ball[index].position.y.high -= 1;
		if (game_work->buttons.held & WS_KEY_Y2) game_work->ball[index].position.x.high += 1;
		if (game_work->buttons.held & WS_KEY_Y3) game_work->ball[index].position.y.high += 1;
		if (game_work->buttons.held & WS_KEY_Y4) game_work->ball[index].position.x.high -= 1;
	}*/
}

uint8_t game_any_ball_alive(void)
{
	for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
	{
		if (game_work->ball[i].is_alive) return 1;
	}
	return 0;
}

uint8_t game_any_brick_alive(void)
{
	for (uint8_t i = 0; i < GAME_BRICKS_MAX_COUNT; i++)
	{
		if (game_work->bricks[i].is_alive) return 1;
	}
	return 0;
}

uint8_t game_check_ball_paddle_collision(const ball* ball, const paddle* paddle)
{
	uint16_t dist_to_center_x = abs(ball->position.x.high + GAME_BALL_HALF_WIDTH - paddle->position.x.high - GAME_PADDLE_HALF_WIDTH);
	uint16_t dist_to_center_y = abs(ball->position.y.high + GAME_BALL_HALF_HEIGHT - paddle->position.y.high - GAME_PADDLE_HALF_HEIGHT);

	if (dist_to_center_x > GAME_PADDLE_HALF_WIDTH + GAME_BALL_RADIUS || dist_to_center_y > GAME_PADDLE_HALF_HEIGHT + GAME_BALL_RADIUS)
		return 0;

	if (dist_to_center_x <= GAME_BRICK_HALF_WIDTH || dist_to_center_y <= GAME_BRICK_HALF_HEIGHT)
		return 1;

	int16_t delta_x = dist_to_center_x - GAME_PADDLE_HALF_WIDTH;
	int16_t delta_y = dist_to_center_y - GAME_PADDLE_HALF_HEIGHT;

	return delta_x * delta_x + delta_y * delta_y <= GAME_BALL_RADIUS * GAME_BALL_RADIUS;
}

uint8_t game_check_ball_brick_collision(const ball* ball, const brick* brick)
{
	uint16_t dist_x = abs(ball->position.x.high + GAME_BALL_HALF_WIDTH - (brick->position.x.high * WS_DISPLAY_TILE_WIDTH) - GAME_BRICK_HALF_WIDTH);
	uint16_t dist_y = abs(ball->position.y.high + GAME_BALL_HALF_HEIGHT - (brick->position.y.high * WS_DISPLAY_TILE_HEIGHT) - GAME_BRICK_HALF_HEIGHT);

	if (dist_x > GAME_BRICK_HALF_WIDTH + GAME_BALL_RADIUS || dist_y > GAME_BRICK_HALF_HEIGHT + GAME_BALL_RADIUS)
		return 0;

	if (dist_x <= GAME_BRICK_HALF_WIDTH || dist_y <= GAME_BRICK_HALF_HEIGHT)
		return 1;

	int16_t delta_x = dist_x - GAME_BRICK_HALF_WIDTH;
	int16_t delta_y = dist_y - GAME_BRICK_HALF_HEIGHT;

	return delta_x * delta_x + delta_y * delta_y <= GAME_BALL_RADIUS * GAME_BALL_RADIUS;
}
