#include "vec2.h"

void vec2_add(const vec2* left, const vec2* right, vec2* out)
{
	out->x.value = left->x.value + right->x.value;
	out->y.value = left->y.value + right->y.value;
}

void vec2_sub(const vec2* left, const vec2* right, vec2* out)
{
	out->x.value = left->x.value - right->x.value;
	out->y.value = left->y.value - right->y.value;
}

void vec2_mul(const vec2* left, const vec2* right, vec2* out)
{
	out->x.value = left->x.value * right->x.value;
	out->y.value = left->y.value * right->y.value;
}

void vec2_div(const vec2* left, const vec2* right, vec2* out)
{
	out->x.value = left->x.value / right->x.value;
	out->y.value = left->y.value / right->y.value;
}
