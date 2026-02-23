#include <stdlib.h>
#include <wonderful.h>
#include <ws.h>

#include "sram.h"

#define SRAM_DEBUG_FORCE_RESET	(0)

const char __far sram_identifier[SRAM_IDENTIFIER_LENGTH] = "WndrflBrknd_SRAM";
const char __far sram_default_hiscore_name[SRAM_HISCORE_NAME_LENGTH] = "Wonderful";
const char __far sram_empty_hiscore_name[SRAM_HISCORE_NAME_LENGTH] = "\0\0\0\0\0\0\0\0\0";
const char __far sram_lazy_hiscore_name[SRAM_HISCORE_NAME_LENGTH] = "AAAAAAAAA";
const char __far sram_default_hiscore_table[SRAM_HISCORE_TABLE_LENGTH][SRAM_HISCORE_NAME_LENGTH] =
{
	"Wonderful",	"xdaniel",
	"Mahiro",		"Momiji",
	"Rin",		"Nadeshiko",
	"An",		"Satsuki",
	"Ikuyo",		"Bocchi",
};

save_data* save = NULL;

uint8_t sram_init(void)
{
	save = calloc(sizeof(save_data), 1);

	memcpy(save, WS_SRAM_MEM, sizeof(save_data));

	if (!sram_is_initialized() || sram_is_damaged())
	{
		sram_perform_reset();
		return 1;
	}

	return 0;
}

void sram_free(void)
{
	free(save);
	save = NULL;
}

uint8_t sram_is_initialized(void)
{
#if SRAM_DEBUG_FORCE_RESET != 0
	#warning Returning 0, FIXME for release!
	return 0;
#endif
	return memcmp(save->identifier, sram_identifier, SRAM_IDENTIFIER_LENGTH) == 0;
}

uint8_t sram_is_damaged(void)
{
#if SRAM_DEBUG_FORCE_RESET != 0
	#warning Returning 1, FIXME for release!
	return 1;
#endif
	return sram_calc_checksum(WS_SRAM_MEM, sizeof(save_data) - 2) != save->checksum;
}

void sram_perform_reset(void)
{
	memset(WS_SRAM_MEM, 0, SRAM_MEMORY_SIZE);

	sram_set_defaults();
	sram_store_to_cartridge();
}

void sram_set_defaults(void)
{
	memcpy(save->identifier, sram_identifier, SRAM_IDENTIFIER_LENGTH);

	for (uint8_t i = 0; i < SRAM_HISCORE_TABLE_LENGTH; i++)
	{
		save->hiscores[i].score = ((uint16_t)(SRAM_HISCORE_TABLE_LENGTH - i) * 5000);
		save->hiscores[i].level = ((uint16_t)(SRAM_HISCORE_TABLE_LENGTH - i) * 1);
		save->hiscores[i].unused = 0;
		memcpy(save->hiscores[i].name, sram_default_hiscore_table[i], SRAM_HISCORE_NAME_LENGTH);
	}

	save->play_count = 1;
	save->checksum = 0;
}

uint8_t sram_get_slot_for_new_score(const uint16_t score)
{
	for (uint8_t i = 0; i < SRAM_HISCORE_TABLE_LENGTH; i++)
	{
		if (score >= save->hiscores[i].score)
			return i;
	}

	return SRAM_HISCORE_INVALID_SLOT;
}

void sram_insert_new_score(const uint16_t score, const uint8_t level, const char name[SRAM_HISCORE_NAME_LENGTH])
{
	uint8_t slot = sram_get_slot_for_new_score(score);
	if (slot == SRAM_HISCORE_INVALID_SLOT) return;

	for (uint8_t i = SRAM_HISCORE_TABLE_LENGTH - 1; i > slot; i--)
		save->hiscores[i] = save->hiscores[i - 1];

	save->hiscores[slot].score = score;
	save->hiscores[slot].level = level;
	memcpy(save->hiscores[slot].name, name, SRAM_HISCORE_NAME_LENGTH);
}

uint16_t sram_calc_checksum(const __far uint8_t* data, const uint16_t count)
{
	uint16_t check = SRAM_CHECKSUM_SEED;
	for (uint8_t i = 0; i < count; i++) 
		check -= *data++;
	return check;
}

void sram_store_to_cartridge(void)
{
	save->checksum = sram_calc_checksum((const uint8_t*)save, sizeof(save_data) - 2);

	memcpy(WS_SRAM_MEM, save, sizeof(save_data));
}
