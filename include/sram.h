#pragma once

#define SRAM_MEMORY_SIZE			(8192)
#define SRAM_CHECKSUM_SEED			(42069)
#define SRAM_IDENTIFIER_LENGTH		(16)
#define SRAM_HISCORE_NAME_LENGTH		(10)
#define SRAM_HISCORE_TABLE_LENGTH		(10)
#define SRAM_HISCORE_INVALID_SLOT		(0xFF)

typedef struct save_data_score_s
{
	uint16_t score;
	uint16_t level;
	uint16_t unused;	// TODO: store game time?
	char name[SRAM_HISCORE_NAME_LENGTH];
} save_data_score;

typedef struct save_data_s
{
	char identifier[SRAM_IDENTIFIER_LENGTH];
	save_data_score hiscores[SRAM_HISCORE_TABLE_LENGTH];
	uint16_t play_count;
	uint16_t checksum;
} save_data;

extern const char __far sram_identifier[];
extern const char __far sram_default_hiscore_name[];
extern const char __far sram_empty_hiscore_name[];
extern const char __far sram_lazy_hiscore_name[];
extern const char __far sram_default_hiscore_table[SRAM_HISCORE_TABLE_LENGTH][SRAM_HISCORE_NAME_LENGTH];

extern save_data* save;

uint8_t sram_init(void);
void sram_free(void);
uint8_t sram_is_initialized(void);
uint8_t sram_is_damaged(void);
void sram_perform_reset(void);
void sram_set_defaults(void);
uint8_t sram_get_slot_for_new_score(const uint16_t score);
void sram_insert_new_score(const uint16_t score, const uint8_t level, const char name[SRAM_HISCORE_NAME_LENGTH]);
uint16_t sram_calc_checksum(const __far uint8_t* data, const uint16_t count);
void sram_store_to_cartridge(void);
