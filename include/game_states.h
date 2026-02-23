#pragma once

enum GAME_STATE
{
	GAME_STATE_STARTUP = 0,

	GAME_STATE_TITLE_SCREEN_INIT,
	GAME_STATE_TITLE_SCREEN,
	GAME_STATE_SCORE_TABLE_INIT,
	GAME_STATE_SCORE_TABLE,
	GAME_STATE_GAMEPLAY_INIT,
	GAME_STATE_GAMEPLAY,
	GAME_STATE_NEXT_LEVEL_INIT,
	GAME_STATE_NEXT_LEVEL,
	GAME_STATE_SCORE_ENTRY_INIT,
	GAME_STATE_SCORE_ENTRY
};

typedef void (*state_function)();
extern const __far state_function game_main_functions[];
extern const __far state_function game_draw_functions[];

void game_main_startup(void);
void game_main_title_screen_init(void);
void game_main_title_screen(void);
void game_main_score_table_init(void);
void game_main_score_table(void);
void game_main_gameplay_init(void);
void game_main_gameplay(void);
void game_main_next_level_init(void);
void game_main_next_level(void);
void game_main_score_entry_init(void);
void game_main_score_entry(void);

void game_draw_startup(void);
void game_draw_title_screen_init(void);
void game_draw_title_screen(void);
void game_draw_score_table_init(void);
void game_draw_score_table(void);
void game_draw_gameplay_init(void);
void game_draw_gameplay(void);
void game_draw_next_level_init(void);
void game_draw_next_level(void);
void game_draw_score_entry_init(void);
void game_draw_score_entry(void);
