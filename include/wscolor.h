#pragma once

typedef struct wscolor_s
{
	union
	{
		struct
		{
			unsigned b : 4;
			unsigned g : 4;
			unsigned r : 4;
			unsigned unused : 4;
		};
		uint16_t rgb;
	};
} wscolor;
