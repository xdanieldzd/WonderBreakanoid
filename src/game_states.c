#include <stdlib.h>
#include <wonderful.h>
#include <ws.h>
#include <wse.h>

#include "game.h"
#include "game_states.h"
#include "game_strings.h"
#include "game_graphics.h"
#include "game_levels.h"

#include "version.h"

#include "main.h"
#include "util.h"
#include "fade.h"
#include "sprite.h"
#include "text.h"
#include "sram.h"

#define GAME_STATES_DEBUG_HISCORE_ENTRY (0)

const __far state_function game_main_functions[] =
{
	game_main_startup,
	game_main_title_screen_init,
	game_main_title_screen,
	game_main_menu_init,
	game_main_menu,
	game_main_score_table_init,
	game_main_score_table,
	game_main_gameplay_init,
	game_main_gameplay,
	game_main_next_level_init,
	game_main_next_level,
	game_main_score_entry_init,
	game_main_score_entry
};

const __far state_function game_draw_functions[] =
{
	game_draw_startup,
	game_draw_title_screen_init,
	game_draw_title_screen,
	game_draw_menu_init,
	game_draw_menu,
	game_draw_score_table_init,
	game_draw_score_table,
	game_draw_gameplay_init,
	game_draw_gameplay,
	game_draw_next_level_init,
	game_draw_next_level,
	game_draw_score_entry_init,
	game_draw_score_entry
};

void game_main_startup(void)
{
	ws_gdma_copy(WS_TILE_4BPP_MEM(0x000), gfx_main_font_tiles, gfx_main_font_tiles_size);

	ws_screen_fill_tiles(&wse_screen2, 0x000 | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(0), 0, 0, WS_SCREEN_WIDTH_TILES, WS_SCREEN_HEIGHT_TILES);

	game_work->sram_was_damaged = sram_init();

	game_work->next_state = GAME_STATE_TITLE_SCREEN_INIT;

#if GAME_STATES_DEBUG_HISCORE_ENTRY != 0
	game_work->has_round_ended = 1;
	game_work->player_has_won = 1;
	game_work->score = 42069;
	game_work->level = 5;
	game_work->next_state = GAME_STATE_NEXT_LEVEL;
#endif
}

void game_main_title_screen_init(void)
{
	game_work->do_process_text_sprites = 1;

	sram_store_to_cartridge();

	game_work->next_graphics = GAME_FOREGROUND_TITLE_SCREEN;
	game_work->next_background = GAME_BACKGROUND_EVENING_LAKE;

	game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);

	game_work->next_state = GAME_STATE_TITLE_SCREEN;
}

void game_main_title_screen(void)
{
	if ((game_work->buttons.held & GAME_DEBUG_ENABLE_KEYS) == GAME_DEBUG_ENABLE_KEYS)
	{
		game_work->debug_enable = 1;
	}
	else
	{
		if (game_work->buttons.pressed & WS_KEY_START)
		{
			game_work->do_process_text_sprites = 0;

			game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
			game_work->next_state = GAME_STATE_MENU_INIT;
		}

		if (game_work->debug_enable && (game_work->buttons.pressed & GAME_SRAM_RESET_KEYS) == GAME_SRAM_RESET_KEYS)
		{
			sram_perform_reset();
			game_work->sram_was_damaged = 1;
		}
	}
}

void game_main_menu_init(void)
{
	game_work->do_process_text_sprites = 1;

	game_work->next_graphics = GAME_FOREGROUND_TITLE_SCREEN;
	game_work->next_background = GAME_BACKGROUND_EVENING_LAKE;

	game_work->main_menu.item_index = 0;
	game_work->main_menu.item_count = 3;

	game_work->main_menu.item_states[0] = GAME_STATE_GAMEPLAY_INIT;
	game_work->main_menu.item_states[1] = GAME_STATE_SCORE_TABLE_INIT;
	game_work->main_menu.item_states[2] = GAME_STATE_TITLE_SCREEN_INIT;

	strcpy(game_work->main_menu.item_labels[0], str_menu_start_game);
	strcpy(game_work->main_menu.item_labels[1], str_menu_hiscores);
	strcpy(game_work->main_menu.item_labels[2], str_menu_exit);

	game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);

	game_work->next_state = GAME_STATE_MENU;
}

void game_main_menu(void)
{
	if (game_work->buttons.pressed & WS_KEY_X1)
	{
		game_work->main_menu.item_index--;
		if (game_work->main_menu.item_index < 0) game_work->main_menu.item_index = game_work->main_menu.item_count - 1;
	}

	if (game_work->buttons.pressed & WS_KEY_X2)
	{
		game_work->main_menu.item_index = game_work->main_menu.item_count - 1;
	}

	if (game_work->buttons.pressed & WS_KEY_X3)
	{
		game_work->main_menu.item_index++;
		if (game_work->main_menu.item_index >= game_work->main_menu.item_count) game_work->main_menu.item_index = 0;
	}

	if (game_work->buttons.pressed & WS_KEY_X4)
	{
		game_work->main_menu.item_index = 0;
	}

	if (game_work->buttons.pressed & (WS_KEY_START | WS_KEY_A))
	{
		game_work->do_process_text_sprites = 0;

		game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
		game_work->next_state = game_work->main_menu.item_states[game_work->main_menu.item_index];
	}

	if (game_work->buttons.pressed & WS_KEY_B)
	{
		game_work->do_process_text_sprites = 0;

		game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
		game_work->next_state = GAME_STATE_TITLE_SCREEN_INIT;
	}
}

void game_main_score_table_init(void)
{
	game_work->do_process_text_sprites = 1;

	game_work->next_graphics = GAME_FOREGROUND_INGAME;
	game_work->next_background = GAME_BACKGROUND_BRIGHT_MOUNTAIN;

	game_clear_screen_with_border();

	game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);

	game_work->next_state = GAME_STATE_SCORE_TABLE;
}

void game_main_score_table(void)
{
	if (game_work->buttons.pressed & (WS_KEY_A | WS_KEY_START))
	{
		game_work->do_process_text_sprites = 0;

		game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
		game_work->next_state = GAME_STATE_MENU_INIT;
	}
}

void game_main_gameplay_init(void)
{
	game_work->do_process_text_sprites = 1;

	game_work->next_graphics = GAME_FOREGROUND_INGAME;
	game_work->next_background = GAME_BACKGROUND_SUNNY_DAY_LAKE;

	game_clear_results();
	game_clear_field();
	game_spawn_bricks(level_data[game_work->level]);

	sram_store_to_cartridge();

	game_begin();

	game_work->timer = SECONDS(3);

	game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);
	game_work->next_state = GAME_STATE_GAMEPLAY;
}

void game_main_gameplay(void)
{
	if (game_work->timer == 0)
	{
		if (!game_work->has_round_ended && game_work->buttons.pressed & WS_KEY_START)
		{
			const uint8_t ignored_pal = 15;
			if (!game_work->is_paused) game_fade_out_to(FADE_MODE_BLACK, WS_REFRESH_RATE_APPROX, 4, &ignored_pal);
			else game_fade_in_from(FADE_MODE_BLACK, WS_REFRESH_RATE_APPROX, 4, &ignored_pal);

			game_work->is_paused = !game_work->is_paused;
		}

		if (game_work->debug_enable && game_work->buttons.pressed & WS_KEY_Y1)
		{
			*WS_DISPLAY_COLOR_MEM(0) = WS_RGB(15, 0, 0);
			for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
				if (game_work->ball[i].is_alive)
					game_work->ball[i].is_piercing = 1;
		}

		if (game_work->debug_enable && game_work->buttons.pressed & WS_KEY_Y3)
		{
			*WS_DISPLAY_COLOR_MEM(0) = WS_RGB(0, 15, 0);
			for (uint8_t i = 0; i < GAME_BRICKS_MAX_COUNT; i++)
				game_work->bricks[i].is_alive = 0;
			game_work->score += 1000;
			game_update_bricks();
		}
	}

	if (game_work->has_round_ended)
	{
		game_work->next_state = GAME_STATE_NEXT_LEVEL_INIT;
	}
	else if (!game_work->is_paused && game_work->timer == 0)
	{
		for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
		{
			if (!game_work->ball[i].is_alive) continue;

			if (game_check_ball_paddle_collision(&game_work->ball[i], &game_work->paddle))
			{
				if (!game_work->paddle.is_sticky)
				{
					game_work->ball[i].velocity.x.value =
						(game_work->ball[i].velocity.x.value / 2) +
						(((game_work->ball[i].position.x.high + GAME_BALL_RADIUS) - (game_work->paddle.position.x.high + GAME_PADDLE_HALF_WIDTH)) << 8) / GAME_PADDLE_HALF_WIDTH;

					game_work->ball[i].velocity.y.value = -abs(game_work->ball[i].velocity.y.value);
				}
				else
				{
					game_work->ball[i].is_on_paddle = 1;
				}
			}

			game_move_ball(i);

			if (!game_work->ball[i].is_on_paddle)
			{
				if (game_work->ball[i].position.y.high >= GAME_PLAYFIELD_MAX_Y_POSITION)
					game_work->ball[i].is_alive = 0;

				for (uint8_t j = 0; j < GAME_BRICKS_MAX_COUNT; j++)
				{
					if (((game_work->bricks[j].type & GAME_BRICK_FLAGS_COLOR_MASK) == GAME_BRICK_FLAGS_COLOR_NONE) || !game_work->bricks[j].is_alive) continue;

					if (game_check_ball_brick_collision(&game_work->ball[i], &game_work->bricks[j]))
					{
						game_work->bricks[j].is_alive = 0;

						if (game_work->score + brick_score_values[game_work->bricks[j].type] < game_work->score)
							game_work->score = UINT16_MAX;
						else
							game_work->score += brick_score_values[game_work->bricks[j].type];

						if (!game_work->ball[i].is_piercing)
						{
							game_work->ball[i].velocity.x.value =
								(game_work->ball[i].velocity.x.value / 2) +
								-game_work->ball[i].velocity.x.value + ((rand() % 8) << 4);

							game_work->ball[i].velocity.y.value = -game_work->ball[i].velocity.y.value;
						}

						game_update_bricks();

						break;
					}
				}
			}
		}

		if (!game_any_ball_alive())
		{
			game_work->paddle.is_alive = 0;
			game_work->has_round_ended = 1;
			game_work->player_has_won = 0;
		}

		if (!game_any_brick_alive())
		{
			game_work->paddle.is_alive = 0;
			game_work->has_round_ended = 1;
			game_work->player_has_won = 1;
		}

		if (game_work->paddle.is_alive)
		{
			uint16_t paddle_movement = 0;

			if (game_work->buttons.held & WS_KEY_X2)
				paddle_movement = game_work->buttons.held & WS_KEY_B ? GAME_PADDLE_FAST_SPEED : GAME_PADDLE_NORMAL_SPEED;

			if (game_work->buttons.held & WS_KEY_X4)
				paddle_movement = game_work->buttons.held & WS_KEY_B ? -GAME_PADDLE_FAST_SPEED : -GAME_PADDLE_NORMAL_SPEED;

			if ((game_work->paddle.position.x.value + paddle_movement) > (GAME_PLAYFIELD_MIN_X_POSITION << 8) && (game_work->paddle.position.x.value + paddle_movement) < (GAME_PLAYFIELD_MAX_X_POSITION - (GAME_PADDLE_WIDTH - 8)) << 8)
				game_work->paddle.position.x.value += paddle_movement;
			else
				paddle_movement = 0;

			if (game_work->paddle.is_sticky)
			{
				for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
				{
					if (game_work->ball[i].is_on_paddle)
					{
						game_work->ball[i].position.x.value += paddle_movement;

						if (game_work->buttons.pressed & WS_KEY_A)
						{
							game_work->ball[i].is_on_paddle = 0;
							game_work->ball[i].velocity.x.value = -game_work->ball[i].velocity.x.value + ((rand() % 8) << 4);
							game_work->ball[i].velocity.y.value = -game_work->ball[i].velocity.y.value;
							game_work->ball[i].position.y.high -= 1;

							game_work->paddle.is_sticky = 0;
						}
					}
				}
			}
		}
	}
}

void game_main_next_level_init(void)
{
	if (game_work->buttons.pressed & WS_KEY_START)
	{
		game_work->do_process_text_sprites = 0;

		game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
		game_work->next_state = GAME_STATE_NEXT_LEVEL;
	}
}

void game_main_next_level(void)
{
	if (game_work->player_has_won) game_work->level++;
	else game_work->lives--;

	if (game_work->lives >= 0 && game_work->level < ARRAY_LENGTH(level_data))
	{
		game_work->do_process_text_sprites = 1;

		game_work->next_graphics = GAME_FOREGROUND_INGAME;
		game_work->next_background = GAME_BACKGROUND_SUNNY_DAY_LAKE;

		if (game_work->player_has_won)
		{
			game_clear_field();
			game_spawn_bricks(level_data[game_work->level]);
		}

		game_begin();

		game_work->timer = SECONDS(3);

		game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);
		game_work->next_state = GAME_STATE_GAMEPLAY;
	}
	else
	{
		if (game_work->level >= ARRAY_LENGTH(level_data))
			game_work->level--;

		if (sram_get_slot_for_new_score(game_work->score) != SRAM_HISCORE_INVALID_SLOT)
		{
			util_generate_sprite_tile_data(game_work->score_entry.cursor_tile_data, GAME_NAME_ENTRY_CURSOR_TILE_COUNT, GAME_NAME_ENTRY_CURSOR_TILE_INDEX, GAME_NAME_ENTRY_CURSOR_PALETTE_INDEX, 1);
			game_clear_screen_with_border();

			game_work->next_state = GAME_STATE_SCORE_ENTRY_INIT;
		}
		else
		{
			game_work->next_state = GAME_STATE_TITLE_SCREEN_INIT;
		}
	}
}

void game_main_score_entry_init(void)
{
	game_work->do_process_text_sprites = 1;

	game_work->next_graphics = GAME_FOREGROUND_INGAME;
	game_work->next_background = GAME_BACKGROUND_BRIGHT_MOUNTAIN;

	memcpy(game_work->score_entry.name_entry, sram_empty_hiscore_name, sizeof(char) * SRAM_HISCORE_NAME_LENGTH);

	game_fade_in_from(FADE_MODE_WHITE, 25, 16, NULL);
	game_work->next_state = GAME_STATE_SCORE_ENTRY;
}

void game_main_score_entry(void)
{
	static char buf[16];

	if (game_work->buttons.pressed & WS_KEY_Y3)
	{
		game_work->score_entry.is_lowercase = !game_work->score_entry.is_lowercase;
		game_draw_score_entry_init();
	}

	if (game_work->buttons.pressed & (WS_KEY_X1 | WS_KEY_X2 | WS_KEY_X3 | WS_KEY_X4))
	{
		if (game_work->buttons.pressed & WS_KEY_X1) game_work->score_entry.cursor_y_position--;
		if (game_work->buttons.pressed & WS_KEY_X2) game_work->score_entry.cursor_x_position++;
		if (game_work->buttons.pressed & WS_KEY_X3) game_work->score_entry.cursor_y_position++;
		if (game_work->buttons.pressed & WS_KEY_X4) game_work->score_entry.cursor_x_position--;

		game_work->score_entry.cursor_x_position %= GAME_NAME_ENTRY_GRID_SIZE;
		game_work->score_entry.cursor_y_position %= (strlen(game_work->score_entry.is_lowercase ? str_scoreentry_character_list_lower : str_scoreentry_character_list_upper) / GAME_NAME_ENTRY_GRID_SIZE);
	}

	if (game_work->buttons.pressed & WS_KEY_A)
	{
		game_work->score_entry.name_entry[game_work->score_entry.name_entry_index] = (game_work->score_entry.is_lowercase ? str_scoreentry_character_list_lower : str_scoreentry_character_list_upper)[game_work->score_entry.cursor_y_position * GAME_NAME_ENTRY_GRID_SIZE + game_work->score_entry.cursor_x_position];
		if (game_work->score_entry.name_entry_index < SRAM_HISCORE_NAME_LENGTH - 2) game_work->score_entry.name_entry_index++;
	}

	if (game_work->buttons.pressed & (WS_KEY_Y2 | WS_KEY_Y4 | WS_KEY_B))
	{
		if (game_work->buttons.pressed & WS_KEY_Y2) game_work->score_entry.name_entry_index++;
		if (game_work->buttons.pressed & WS_KEY_Y4) game_work->score_entry.name_entry_index--;

		uint8_t name_length = strlen(game_work->score_entry.name_entry);
		if (name_length > SRAM_HISCORE_NAME_LENGTH - 2) name_length = SRAM_HISCORE_NAME_LENGTH - 2;

		if (game_work->score_entry.name_entry_index < 0) game_work->score_entry.name_entry_index = name_length;
		if (game_work->score_entry.name_entry_index > name_length) game_work->score_entry.name_entry_index = 0;

		if (game_work->buttons.pressed & WS_KEY_B)
		{
			if (game_work->score_entry.name_entry_index < name_length)
			{
				strcpy(buf, &game_work->score_entry.name_entry[game_work->score_entry.name_entry_index + 1]);
				strcpy(&game_work->score_entry.name_entry[game_work->score_entry.name_entry_index], buf);
			}
			else
			{
				if (game_work->score_entry.name_entry_index == SRAM_HISCORE_NAME_LENGTH - 2 && game_work->score_entry.name_entry[game_work->score_entry.name_entry_index] != '\0')
					game_work->score_entry.name_entry[game_work->score_entry.name_entry_index] = '\0';
				else
					game_work->score_entry.name_entry[--game_work->score_entry.name_entry_index] = '\0';
			}
		}
	}

	if (game_work->buttons.pressed & WS_KEY_START)
	{
		if (	memcmp(game_work->score_entry.name_entry, sram_empty_hiscore_name, SRAM_HISCORE_NAME_LENGTH - 1) == 0 ||
			memcmp(game_work->score_entry.name_entry, sram_lazy_hiscore_name, SRAM_HISCORE_NAME_LENGTH - 1) == 0)
		{
			strcpy(game_work->score_entry.name_entry, sram_default_hiscore_name);
			game_work->score_entry.name_entry_index = SRAM_HISCORE_NAME_LENGTH - 2;
		}
		else
		{
			game_work->do_process_text_sprites = 0;

			sram_insert_new_score(game_work->score, game_work->level, game_work->score_entry.name_entry);
			sram_store_to_cartridge();

			game_fade_out_to(FADE_MODE_WHITE, 25, 16, NULL);
			game_work->next_state = GAME_STATE_TITLE_SCREEN_INIT;
		}
	}
}

void game_draw(void)
{
	if (game_draw_functions[game_work->current_state] != NULL)
		(*game_draw_functions[game_work->current_state])();	
}

void game_draw_startup(void)
{
	//
}

void game_draw_title_screen_init(void)
{
	//
}

void game_draw_title_screen(void)
{
	text_print(4, 4, TEXT_FLAGS_SPRITES, str_title_tagline_1);
	text_print(WS_DISPLAY_WIDTH_PIXELS - GAME_STRING_WIDTH_PIXELS(str_title_tagline_2) - 4, 12, TEXT_FLAGS_SPRITES, str_title_tagline_2);

	if ((vbl_ticks % WS_REFRESH_RATE_APPROX) >= 15)
		text_print(GAME_MESSAGE_X_POSITION(str_title_press_start), 104, TEXT_FLAGS_SPRITES, str_title_press_start);

	text_print(GAME_MESSAGE_X_POSITION(str_title_credits_1), 124, TEXT_FLAGS_SPRITES, str_title_credits_1);
	text_print(GAME_MESSAGE_X_POSITION(str_title_credits_2), 132, TEXT_FLAGS_SPRITES, str_title_credits_2);

	if (game_work->sram_was_damaged) text_print(0, 0, TEXT_FLAGS_BACKGROUND, str_sram_reset);
	if (game_work->debug_enable) text_print(0, 11, TEXT_FLAGS_BACKGROUND, str_dbg_version, build_date);
}

void game_draw_menu_init(void)
{
	//
}

void game_draw_menu(void)
{
	text_print(4, 4, TEXT_FLAGS_SPRITES, str_title_tagline_1);
	text_print(WS_DISPLAY_WIDTH_PIXELS - GAME_STRING_WIDTH_PIXELS(str_title_tagline_2) - 4, 12, TEXT_FLAGS_SPRITES, str_title_tagline_2);

	uint8_t y = WS_DISPLAY_HEIGHT_PIXELS - 20 - ((game_work->main_menu.item_count - game_work->main_menu.item_index) * 10);
	text_print(36, y, TEXT_FLAGS_SPRITES, (const char __far*)&"\x7F");
	text_print(WS_DISPLAY_WIDTH_PIXELS - 36 - WS_DISPLAY_TILE_WIDTH, y, TEXT_FLAGS_SPRITES, (const char __far*)&"\x7F");

	for (uint8_t i = 0; i < game_work->main_menu.item_count; i++)
	{
		y = WS_DISPLAY_HEIGHT_PIXELS - 20 - (((game_work->main_menu.item_count - i) * 10));
		text_print(GAME_MESSAGE_X_POSITION(game_work->main_menu.item_labels[i]), y, TEXT_FLAGS_SPRITES, game_work->main_menu.item_labels[i]);
	}

	if ((vbl_ticks % WS_REFRESH_RATE_APPROX) >= 15)
		text_print(GAME_MESSAGE_X_POSITION(str_menu_press_a_start), 128, TEXT_FLAGS_SPRITES, str_menu_press_a_start);
}

void game_draw_score_table_init(void)
{
	text_print(9, 0, TEXT_FLAGS_BACKGROUND, str_hiscores_header);
	text_print(2, 2, TEXT_FLAGS_BACKGROUND, str_hiscores_list_header);

	for (uint8_t i = 0; i < SRAM_HISCORE_TABLE_LENGTH; i++)
		text_print(2, (i + 4), TEXT_FLAGS_BACKGROUND, str_hiscores_score_entry, (i + 1), save->hiscores[i].score, save->hiscores[i].level, (const char __far*)&save->hiscores[i].name);
}

void game_draw_score_table(void)
{
	if ((vbl_ticks % WS_REFRESH_RATE_APPROX) >= 15)
		text_print(GAME_MESSAGE_X_POSITION(str_hiscores_press_to_exit), 120, TEXT_FLAGS_SPRITES, str_hiscores_press_to_exit);
}

void game_draw_gameplay_init(void)
{
	//
}

void game_draw_gameplay(void)
{
	text_print(11, 0, TEXT_FLAGS_BACKGROUND, str_score, game_work->score);

	if (game_work->timer == 0)
	{
		if (game_work->is_paused)
		{
			if ((vbl_ticks % WS_REFRESH_RATE_APPROX) >= 15)
				text_print(GAME_MESSAGE_X_POSITION(str_paused), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_paused);

			text_print(32, GAME_MESSAGE_Y_POSITION + 12, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_level, game_work->level + 1);
			text_print(112, GAME_MESSAGE_Y_POSITION + 12, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_lives, game_work->lives < 0 ? 0 : game_work->lives);
		}
	}
	else
	{
		if (game_work->timer >= SECONDS(1))
		{
			if ((vbl_ticks % WS_REFRESH_RATE_APPROX) >= 15)
			{
				text_print(GAME_MESSAGE_X_POSITION(str_get_ready), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_get_ready);
			}
		}
		else
			text_print(GAME_MESSAGE_X_POSITION(str_go), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_go);

		text_print(32, GAME_MESSAGE_Y_POSITION + 12, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_level, game_work->level + 1);
		text_print(112, GAME_MESSAGE_Y_POSITION + 12, TEXT_FLAGS_SPRITES | TEXT_FLAGS_SPRITES_HAVE_PRIO, str_lives, game_work->lives < 0 ? 0 : game_work->lives);
	}

	sprite_set_multi(&game_work->paddle.position, (struct vec2_s) { .x.high = 8, .y.high = 0 }, game_work->paddle.tile_data, ARRAY_LENGTH(game_work->paddle.tile_data));

	for (uint8_t i = 0; i < GAME_BALLS_MAX_COUNT; i++)
		if (game_work->ball[i].is_alive)
			sprite_set(&game_work->ball[i].position, game_work->ball[i].tile_data);
}

void game_draw_next_level_init(void)
{
	game_draw_gameplay();

	if (game_work->player_has_won)
	{
		text_print(GAME_MESSAGE_X_POSITION(str_level_complete), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES, str_level_complete);
		text_print(GAME_MESSAGE_X_POSITION(str_press_retry), GAME_MESSAGE_Y_POSITION + 10, TEXT_FLAGS_SPRITES, str_press_retry);
	}
	else
	{
		if (game_work->lives > 0)
			text_print(GAME_MESSAGE_X_POSITION(str_press_retry), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES, str_press_retry);
		else
		{
			text_print(GAME_MESSAGE_X_POSITION(str_game_over), GAME_MESSAGE_Y_POSITION, TEXT_FLAGS_SPRITES, str_game_over);
			text_print(GAME_MESSAGE_X_POSITION(str_press_retry), GAME_MESSAGE_Y_POSITION + 10, TEXT_FLAGS_SPRITES, str_press_retry);
		}
	}
}

void game_draw_next_level(void)
{
	//
}

void game_draw_score_entry_init(void)
{
	text_print(5, 0, TEXT_FLAGS_BACKGROUND, str_scoreentry_header);
	text_print(2, 2, TEXT_FLAGS_BACKGROUND, str_scoreentry_entry, game_work->score, game_work->level + 1);

	const char __far * ptr = game_work->score_entry.is_lowercase ? str_scoreentry_character_list_lower : str_scoreentry_character_list_upper;
	uint8_t i = 0;

	for (; *ptr != '\0'; ptr++)
	{
		ws_screen_put_tile(
			&wse_screen2, (*ptr - 0x020) | WS_SCREEN_ATTR_BANK(0) | WS_SCREEN_ATTR_PALETTE(15),
			GAME_NAME_ENTRY_GRID_X_POSITION + ((i % GAME_NAME_ENTRY_GRID_SIZE) * GAME_NAME_ENTRY_GRID_X_SPACING),
			GAME_NAME_ENTRY_GRID_Y_POSITION + ((i / GAME_NAME_ENTRY_GRID_SIZE) * GAME_NAME_ENTRY_GRID_Y_SPACING));
		i++;
	}
}

void game_draw_score_entry(void)
{
	//text_print(0,0,TEXT_FLAGS_BACKGROUND,"%3u %3u",game_work->score_entry.cursor_x_position,game_work->score_entry.cursor_y_position);

	for (uint8_t i = 0; i < SRAM_HISCORE_NAME_LENGTH - 1; i++)
	{
		text_print(
			GAME_NAME_ENTRY_LETTERS_X_POSITION + i * WS_DISPLAY_TILE_WIDTH,
			GAME_NAME_ENTRY_LETTERS_Y_POSITION,
			TEXT_FLAGS_SPRITES,
			str_fmt_char,
			game_work->score_entry.name_entry[i] == '\0' ? '-' : game_work->score_entry.name_entry[i]);

		if (i == game_work->score_entry.name_entry_index)
		{
			sprite_set(
				&(struct vec2_s)
				{
					.x.high = GAME_NAME_ENTRY_LETTERS_X_POSITION + i * WS_DISPLAY_TILE_WIDTH,
					.y.high = GAME_NAME_ENTRY_LETTERS_Y_POSITION + 2
				},
				game_work->score_entry.cursor_tile_data);
		}
	}

	if ((vbl_ticks % GAME_NAME_ENTRY_CONTROLS_DURATION) < GAME_NAME_ENTRY_CONTROLS_SWITCH_1) text_print(GAME_MESSAGE_X_POSITION(str_scoreentry_controls_1), 120, TEXT_FLAGS_SPRITES, str_scoreentry_controls_1);
	else if ((vbl_ticks % GAME_NAME_ENTRY_CONTROLS_DURATION) < GAME_NAME_ENTRY_CONTROLS_SWITCH_2) text_print(GAME_MESSAGE_X_POSITION(str_scoreentry_controls_2), 120, TEXT_FLAGS_SPRITES, str_scoreentry_controls_2);
	else if ((vbl_ticks % GAME_NAME_ENTRY_CONTROLS_DURATION) < GAME_NAME_ENTRY_CONTROLS_SWITCH_3) text_print(GAME_MESSAGE_X_POSITION(str_scoreentry_controls_3), 120, TEXT_FLAGS_SPRITES, str_scoreentry_controls_3);
	else if ((vbl_ticks % GAME_NAME_ENTRY_CONTROLS_DURATION) < GAME_NAME_ENTRY_CONTROLS_SWITCH_4) text_print(GAME_MESSAGE_X_POSITION(str_scoreentry_controls_4), 120, TEXT_FLAGS_SPRITES, str_scoreentry_controls_4);
	else text_print(GAME_MESSAGE_X_POSITION(str_scoreentry_controls_5), 120, TEXT_FLAGS_SPRITES, str_scoreentry_controls_5);

	if ((vbl_ticks % GAME_NAME_ENTRY_CURSOR_BLINK_SPEED) >= GAME_NAME_ENTRY_CURSOR_PERIOD)
		sprite_set(
			&(struct vec2_s)
			{
				.x.high = (GAME_NAME_ENTRY_GRID_X_POSITION + ((game_work->score_entry.cursor_x_position % GAME_NAME_ENTRY_GRID_SIZE) * GAME_NAME_ENTRY_GRID_X_SPACING)) * WS_DISPLAY_TILE_WIDTH,
				.y.high = ((GAME_NAME_ENTRY_GRID_Y_POSITION + ((game_work->score_entry.cursor_y_position % GAME_NAME_ENTRY_GRID_SIZE) * GAME_NAME_ENTRY_GRID_Y_SPACING)) * WS_DISPLAY_TILE_HEIGHT) + 2
			},
			game_work->score_entry.cursor_tile_data);
}
