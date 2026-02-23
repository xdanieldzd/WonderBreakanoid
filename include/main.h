#pragma once

#define ARRAY_LENGTH(array)		(*(&array + 1) - array)

#define WS_REFRESH_RATE_APPROX	(75)

extern volatile uint16_t vbl_ticks;

void enable_interrupts(uint8_t interrupts);
void disable_interrupts(void);
void wait_for_vblank(void);
