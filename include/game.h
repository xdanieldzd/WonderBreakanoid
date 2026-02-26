#pragma once

#include "vec2.h"
#include "sram.h"

#define WIDTH(tile_count)					((tile_count) * WS_DISPLAY_TILE_WIDTH)
#define HEIGHT(tile_count)					((tile_count) * WS_DISPLAY_TILE_HEIGHT)
#define HALF(value)						((value) / 2)

#define GAME_DEBUG_ENABLE_KEYS				(WS_KEY_Y1 | WS_KEY_A)
#define GAME_SRAM_RESET_KEYS					(WS_KEY_Y2 | WS_KEY_Y4)

#define GAME_MENU_ENTRY_MAX_COUNT				(4)
#define GAME_MENU_LABEL_MAX_LENGTH			(16)

#define GAME_PADDLE_FIRST_TILE_INDEX			(0x060)
#define GAME_PADDLE_TILE_COUNT				(5)
#define GAME_PADDLE_PALETTE_INDEX				(8)
#define GAME_PADDLE_WIDTH					WIDTH(GAME_PADDLE_TILE_COUNT)
#define GAME_PADDLE_HEIGHT					HEIGHT(1)
#define GAME_PADDLE_HALF_WIDTH				HALF(GAME_PADDLE_WIDTH)
#define GAME_PADDLE_HALF_HEIGHT				HALF(GAME_PADDLE_HEIGHT)

#define GAME_BALL_FIRST_TILE_INDEX			(GAME_PADDLE_FIRST_TILE_INDEX + GAME_PADDLE_TILE_COUNT)
#define GAME_BALL_TILE_COUNT					(1)
#define GAME_BALL_PALETTE_INDEX				(8)
#define GAME_BALL_WIDTH						WIDTH(GAME_BALL_TILE_COUNT)
#define GAME_BALL_HEIGHT					HEIGHT(1)
#define GAME_BALL_HALF_WIDTH					HALF(GAME_BALL_WIDTH)
#define GAME_BALL_HALF_HEIGHT				HALF(GAME_BALL_HEIGHT)

#define GAME_BORDER_FIRST_TILE_INDEX			(GAME_BALL_FIRST_TILE_INDEX + GAME_BALL_TILE_COUNT)

#define GAME_BRICK_FIRST_TILE_INDEX			(0x070)
#define GAME_BRICK_TILE_COUNT				(2)
#define GAME_BRICK_PALETTE_INDEX				(8)
#define GAME_BRICK_WIDTH					WIDTH(GAME_BRICK_TILE_COUNT)
#define GAME_BRICK_HEIGHT					HEIGHT(1)
#define GAME_BRICK_HALF_WIDTH				HALF(GAME_BRICK_WIDTH)
#define GAME_BRICK_HALF_HEIGHT				HALF(GAME_BRICK_HEIGHT)

#define GAME_PADDLE_SPAWN_X_POSITION			HALF(WS_DISPLAY_WIDTH_PIXELS - GAME_PADDLE_WIDTH)
#define GAME_PADDLE_SPAWN_Y_POSITION			(WS_DISPLAY_HEIGHT_PIXELS - GAME_PADDLE_HEIGHT - 4)
#define GAME_PADDLE_NORMAL_SPEED				(0x0200)
#define GAME_PADDLE_FAST_SPEED				(0x0380)

#define GAME_BALL_RADIUS					(3)
#define GAME_BALL_MAX_VELOCITY				(0x0400)

#define GAME_BORDER_DRAW_LEFT				(1 << 0)
#define GAME_BORDER_DRAW_RIGHT				(1 << 1)
#define GAME_BORDER_DRAW_TOP					(1 << 2)
#define GAME_BORDER_DRAW_BOTTOM				(1 << 3)
#define GAME_BORDER_DRAW_LARGE				(1 << 4)

#define GAME_BRICK_FLAGS_COLOR_NONE			(0x00)
#define GAME_BRICK_FLAGS_COLOR_WHITE			(0x01)
#define GAME_BRICK_FLAGS_COLOR_RED			(0x02)
#define GAME_BRICK_FLAGS_COLOR_GREEN			(0x03)
#define GAME_BRICK_FLAGS_COLOR_BLUE			(0x04)
#define GAME_BRICK_FLAGS_COLOR_ORANGE			(0x05)
#define GAME_BRICK_FLAGS_COLOR_YELLOW			(0x06)
#define GAME_BRICK_FLAGS_COLOR_MASK			(0x07)

#define GAME_PLAYFIELD_MIN_X_POSITION			(8)
#define GAME_PLAYFIELD_MIN_Y_POSITION			(8)
#define GAME_PLAYFIELD_MAX_X_POSITION			(WS_DISPLAY_WIDTH_PIXELS - 16)
#define GAME_PLAYFIELD_MAX_Y_POSITION			(WS_DISPLAY_HEIGHT_PIXELS)

#define GAME_STRING_WIDTH_PIXELS(str)			(strlen(str) * WS_DISPLAY_TILE_WIDTH)
#define GAME_MESSAGE_X_POSITION(str)			HALF(WS_DISPLAY_WIDTH_PIXELS - GAME_STRING_WIDTH_PIXELS(str))
#define GAME_MESSAGE_Y_POSITION				(HALF(WS_DISPLAY_HEIGHT_PIXELS) - 12)

#define GAME_NAME_ENTRY_GRID_X_POSITION		(3)
#define GAME_NAME_ENTRY_GRID_Y_POSITION		(7)
#define GAME_NAME_ENTRY_GRID_SIZE				(8)
#define GAME_NAME_ENTRY_GRID_X_SPACING			(3)
#define GAME_NAME_ENTRY_GRID_Y_SPACING			(2)

#define GAME_NAME_ENTRY_CURSOR_TILE_INDEX		(0x03F)
#define GAME_NAME_ENTRY_CURSOR_TILE_COUNT		(1)
#define GAME_NAME_ENTRY_CURSOR_PALETTE_INDEX	(15)
#define GAME_NAME_ENTRY_LETTERS_X_POSITION		HALF(WS_DISPLAY_WIDTH_PIXELS - WIDTH(SRAM_HISCORE_NAME_LENGTH - 1))
#define GAME_NAME_ENTRY_LETTERS_Y_POSITION		(HEIGHT(4) + (WS_DISPLAY_TILE_HEIGHT / 3))

#define GAME_NAME_ENTRY_CONTROLS_DURATION		(WS_REFRESH_RATE_APPROX * 10)
#define GAME_NAME_ENTRY_CONTROLS_SWITCH_1		((GAME_NAME_ENTRY_CONTROLS_DURATION / 10) * 2)
#define GAME_NAME_ENTRY_CONTROLS_SWITCH_2		((GAME_NAME_ENTRY_CONTROLS_DURATION / 10) * 4)
#define GAME_NAME_ENTRY_CONTROLS_SWITCH_3		((GAME_NAME_ENTRY_CONTROLS_DURATION / 10) * 6)
#define GAME_NAME_ENTRY_CONTROLS_SWITCH_4		((GAME_NAME_ENTRY_CONTROLS_DURATION / 10) * 8)

#define GAME_NAME_ENTRY_CURSOR_BLINK_SPEED		(WS_REFRESH_RATE_APPROX / 3)
#define GAME_NAME_ENTRY_CURSOR_PERIOD			HALF(GAME_NAME_ENTRY_CURSOR_BLINK_SPEED)

#define GAME_BRICKS_FIELD_WIDTH				(13)
#define GAME_BRICKS_FIELD_HEIGHT				(9)

#define GAME_BALLS_MAX_COUNT					(4)
#define GAME_BRICKS_MAX_COUNT				(GAME_BRICKS_FIELD_WIDTH * GAME_BRICKS_FIELD_HEIGHT)

enum GAME_FOREGROUND
{
	GAME_FOREGROUND_TITLE_SCREEN = 0,
	GAME_FOREGROUND_INGAME
};

enum GAME_BACKGROUND
{
	GAME_BACKGROUND_SUNNY_DAY_LAKE = 0,
	GAME_BACKGROUND_EVENING_LAKE,
	GAME_BACKGROUND_BRIGHT_MOUNTAIN
};

typedef struct buttons_s
{
	uint16_t held;
	uint16_t pressed;
	uint16_t held_last;
} buttons;

typedef struct paddle_s
{
	vec2 position;
	union
	{
		struct
		{
			unsigned is_alive : 1;
			unsigned is_sticky : 1;
			unsigned unused : 6;
		};
		uint8_t flags;
	};
	uint16_t tile_data[GAME_PADDLE_TILE_COUNT];
} paddle;

typedef struct ball_s
{
	vec2 position, velocity;
	union
	{
		struct
		{
			unsigned is_alive : 1;
			unsigned is_on_paddle : 1;
			unsigned is_piercing : 1;
			unsigned unused : 5;
		};
		uint8_t flags;
	};
	uint16_t tile_data[GAME_BALL_TILE_COUNT];
} ball;

typedef struct brick_s
{
	vec2 position;
	union
	{
		struct
		{
			unsigned is_alive : 1;
			unsigned unused : 5;
			unsigned type : 3;
		};
		uint8_t flags;
	};
	uint16_t tile_data[GAME_BRICK_TILE_COUNT];
} brick;

typedef struct score_entry_s
{
	uint8_t cursor_x_position;
	uint8_t cursor_y_position;
	uint8_t is_lowercase;
	uint16_t cursor_tile_data[GAME_BALL_TILE_COUNT];
	int8_t name_entry_index;
	char name_entry[SRAM_HISCORE_NAME_LENGTH];
} score_entry;

typedef struct main_menu_s
{
	int8_t item_index;
	uint8_t item_count;
	uint8_t item_states[GAME_MENU_ENTRY_MAX_COUNT];
	char item_labels[GAME_MENU_ENTRY_MAX_COUNT][GAME_MENU_LABEL_MAX_LENGTH];
} main_menu;

typedef struct graphics_data_s
{
	const uint8_t __far* tiles;
	const uint8_t __far* palette;
	const uint8_t __far* map;
	const uint16_t tiles_size;
	const uint16_t palette_size;
	const uint16_t map_size;
} graphics_data;

typedef struct game_s
{
	uint16_t random_seed;
	uint8_t sram_was_damaged;

	buttons buttons;

	uint8_t current_state;
	uint8_t next_state;

	main_menu main_menu;

	uint8_t timer;

	uint8_t current_background;
	uint8_t next_background;
	uint8_t current_graphics;
	uint8_t next_graphics;

	paddle paddle;
	ball ball[GAME_BALLS_MAX_COUNT];
	brick bricks[GAME_BRICKS_MAX_COUNT];

	uint16_t score;
	uint8_t level;
	int8_t lives;
	union
	{
		struct
		{
			unsigned debug_enable : 1;
			unsigned is_paused : 1;
			unsigned has_round_ended : 1;
			unsigned player_has_won : 1;
			unsigned do_process_text_sprites : 1;
			unsigned unused : 3;
		};
		uint8_t game_flags;
	};

	score_entry score_entry;
} game;

extern game* game_work;

extern const uint8_t __far* level_data[6];

extern const uint8_t brick_score_values[5];

void game_init(void);
void game_main(void);
void game_draw(void);
void game_exit(void);
__attribute__((assume_ss_data, interrupt)) void __far game_vblank_int_handler(void);
__attribute__((assume_ss_data, interrupt)) void __far game_line_match_int_handler(void);

void game_fade_in_from(const uint8_t mode, const uint8_t divider, const uint8_t until, const uint8_t* ignored_pal);
void game_fade_out_to(const uint8_t mode, const uint8_t divider, const uint8_t until, const uint8_t* ignored_pal);
void game_load_graphics(uint8_t graphics);
void game_load_background(uint8_t background);
void game_draw_border_lines(uint8_t data);
void game_clear_screen_with_border(void);
void game_clear_field(void);
void game_clear_results(void);
void game_begin();
void game_spawn_paddle(void);
void game_spawn_ball(vec2 position);
void game_spawn_bricks(const __far uint8_t* bricks);
void game_update_bricks(void);
void game_move_ball(const uint8_t index);
uint8_t game_any_ball_alive(void);
uint8_t game_any_brick_alive(void);
uint8_t game_check_ball_paddle_collision(const ball* ball, const paddle* paddle);
uint8_t game_check_ball_brick_collision(const ball* ball, const brick* brick);
