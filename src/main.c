#include <wonderful.h>
#include <ws.h>
#include <wse.h>

#include "main.h"
#include "game.h"

volatile uint16_t vbl_ticks;

void main(void)
{
	disable_interrupts();

	ws_system_set_mode(WS_MODE_COLOR_4BPP);

	ws_display_set_screen_addresses(&wse_screen1, &wse_screen2);
	ws_display_set_sprite_address(&wse_sprites1);

	ws_display_scroll_screen1_to(0, 0);
	ws_display_scroll_screen2_to(0, 0);

	ws_screen_fill_tiles(&wse_screen1, 0x000 | WS_SCREEN_ATTR_BANK(1) | WS_SCREEN_ATTR_PALETTE(0), 0, 0, WS_SCREEN_WIDTH_TILES, WS_SCREEN_HEIGHT_TILES);
	ws_screen_fill_tiles(&wse_screen2, 0x000 | WS_SCREEN_ATTR_BANK(1) | WS_SCREEN_ATTR_PALETTE(0), 0, 0, WS_SCREEN_WIDTH_TILES, WS_SCREEN_HEIGHT_TILES);

	ws_display_set_control(WS_DISPLAY_CTRL_SCR1_ENABLE | WS_DISPLAY_CTRL_SCR2_ENABLE | WS_DISPLAY_CTRL_SPR_ENABLE);

	game_init();

	enable_interrupts(WS_INT_ENABLE_VBLANK);

	while (1)
	{
		game_main();
		wait_for_vblank();
	}

	game_exit();
}

void enable_interrupts(uint8_t interrupts)
{
    ws_int_ack_all();
	ws_int_enable(interrupts);
	ia16_enable_irq();
}

void disable_interrupts(void)
{
	ws_int_disable_all();
	ia16_disable_irq();
}

void wait_for_vblank(void)
{
	uint16_t last_vbl_ticks = vbl_ticks;
	while (last_vbl_ticks == vbl_ticks) ia16_halt();
}
