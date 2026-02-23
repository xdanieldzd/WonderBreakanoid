#pragma once

#include <wonderful.h>

typedef union high_low_u
{
	int16_t value;
	struct
	{
		uint8_t low;
		uint8_t high;
	};
	struct
	{
		int8_t low_signed;
		int8_t high_signed;
	};
} high_low;

typedef struct vec2_s
{
	high_low x;
	high_low y;
} vec2;

void vec2_add(const vec2* left, const vec2* right, vec2* out);
void vec2_sub(const vec2* left, const vec2* right, vec2* out);
void vec2_mul(const vec2* left, const vec2* right, vec2* out);
void vec2_div(const vec2* left, const vec2* right, vec2* out);
