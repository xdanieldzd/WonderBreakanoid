#pragma once

#define STRING __attribute__((section(".farrodata"))) const char __far

extern STRING str_ingame_score[];
extern STRING str_ingame_lives[];
extern STRING str_ingame_round[];
extern STRING str_ingame_round_complete[];
extern STRING str_ingame_press_to_retry[];
extern STRING str_ingame_press_to_continue[];
extern STRING str_ingame_game_over[];
extern STRING str_ingame_game_paused[];
extern STRING str_ingame_get_ready[];
extern STRING str_ingame_go[];
extern STRING str_title_dbg_version[];
extern STRING str_title_sram_reset[];
extern STRING str_title_tagline_1[];
extern STRING str_title_tagline_2[];
extern STRING str_title_press_start[];
extern STRING str_title_credits_1[];
extern STRING str_title_credits_2[];
extern STRING str_hiscores_header[];
extern STRING str_hiscores_list_header[];
extern STRING str_hiscores_score_entry[];
extern STRING str_hiscores_press_to_exit[];
extern STRING str_scoreentry_header[];
extern STRING str_scoreentry_controls_1[];
extern STRING str_scoreentry_controls_2[];
extern STRING str_scoreentry_controls_3[];
extern STRING str_scoreentry_controls_4[];
extern STRING str_scoreentry_controls_5[];
extern STRING str_scoreentry_controls_6[];
extern STRING str_scoreentry_fmt_char[];
extern STRING str_scoreentry_entry[];
extern STRING str_scoreentry_character_list_upper[];
extern STRING str_scoreentry_character_list_lower[];
extern STRING str_menu_start_new_game[];
extern STRING str_menu_check_hiscores[];
extern STRING str_menu_return_to_title[];
extern STRING str_menu_press_to_select[];
