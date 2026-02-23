#pragma once

enum GAME_STATE
{
	GAME_STATE_INIT = 0,
	GAME_STATE_INIT_TITLE,
	GAME_STATE_TITLE,
	GAME_STATE_INIT_SCORES,
	GAME_STATE_SCORES,

	GAME_STATE_NEW_GAME,			// TODO: refactor/rename all ingame related states?
	GAME_STATE_ENTER_GAME,
	GAME_STATE_INGAME,
	GAME_STATE_WIN,
	GAME_STATE_LOSE,
	GAME_STATE_CHECK_NEXT_LEVEL,

	GAME_STATE_INIT_SCORE_ENTRY,
	GAME_STATE_SCORE_ENTRY,

	GAME_STATE_INVALID = -1
};

typedef void (*state_function)();
extern const __far state_function game_main_functions[];
extern const __far state_function game_draw_functions[];

void game_main_init(void);
void game_main_init_title(void);
void game_main_title(void);
void game_main_init_scores(void);
void game_main_scores(void);
void game_main_new_game(void);
void game_main_enter_game(void);
void game_main_ingame(void);
void game_main_win(void);
void game_main_lose(void);
void game_main_check_next_level(void);
void game_main_init_score_entry(void);
void game_main_score_entry(void);

void game_draw_init(void);
void game_draw_init_title(void);
void game_draw_title(void);
void game_draw_init_scores(void);
void game_draw_scores(void);
void game_draw_new_game(void);
void game_draw_enter_game(void);
void game_draw_ingame(void);
void game_draw_win(void);
void game_draw_lose(void);
void game_draw_check_next_level(void);
void game_draw_init_score_entry(void);
void game_draw_score_entry(void);
