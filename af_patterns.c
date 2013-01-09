/**
 * $Revision$
 * $Date$
 * $Author$
 */

#include <stdbool.h>

#include <camera.h>

#include "macros.h"
#include "main.h"
#include "firmware.h"

#include "utils.h"

#include "af_patterns.h"

type_PATTERN_MAP_ITEM pattern_map[] = {
		{AF_PATTERN_CENTER,         AF_PATTERN_SQUARE, AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},
		{AF_PATTERN_SQUARE,         AF_PATTERN_HLINE,  AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},

		{AF_PATTERN_TOP,            AF_PATTERN_CENTER, AF_PATTERN_TOP,            AF_PATTERN_TOPTRIANGLE,    AF_PATTERN_TOPLEFT,       AF_PATTERN_TOPRIGHT},
		{AF_PATTERN_TOPTRIANGLE,    AF_PATTERN_CENTER, AF_PATTERN_TOP,            AF_PATTERN_TOPDIAMOND,     AF_PATTERN_LEFTTRIANGLE,  AF_PATTERN_RIGHTTRIANGLE},
		{AF_PATTERN_TOPDIAMOND,     AF_PATTERN_CENTER, AF_PATTERN_TOPTRIANGLE,    AF_PATTERN_TOPHALF,        AF_PATTERN_LEFTDIAMOND,   AF_PATTERN_RIGHTDIAMOND},
		{AF_PATTERN_TOPHALF,        AF_PATTERN_CENTER, AF_PATTERN_TOPDIAMOND,     AF_PATTERN_HLINE,          AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},

		{AF_PATTERN_BOTTOM,         AF_PATTERN_CENTER, AF_PATTERN_BOTTOMTRIANGLE, AF_PATTERN_BOTTOM,         AF_PATTERN_BOTTOMLEFT,    AF_PATTERN_BOTTOMRIGHT},
		{AF_PATTERN_BOTTOMTRIANGLE, AF_PATTERN_CENTER, AF_PATTERN_BOTTOMDIAMOND,  AF_PATTERN_BOTTOM,         AF_PATTERN_LEFTTRIANGLE,  AF_PATTERN_RIGHTTRIANGLE},
		{AF_PATTERN_BOTTOMDIAMOND,  AF_PATTERN_CENTER, AF_PATTERN_BOTTOMHALF,     AF_PATTERN_BOTTOMTRIANGLE, AF_PATTERN_LEFTDIAMOND,   AF_PATTERN_RIGHTDIAMOND},
		{AF_PATTERN_BOTTOMHALF,     AF_PATTERN_CENTER, AF_PATTERN_HLINE,          AF_PATTERN_BOTTOMDIAMOND,  AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},

		{AF_PATTERN_TOPLEFT,        AF_PATTERN_CENTER, AF_PATTERN_TOP,            AF_PATTERN_BOTTOMLEFT,     AF_PATTERN_LEFT,          AF_PATTERN_TOPRIGHT},
		{AF_PATTERN_TOPRIGHT,       AF_PATTERN_CENTER, AF_PATTERN_TOP,            AF_PATTERN_BOTTOMRIGHT,    AF_PATTERN_TOPLEFT,       AF_PATTERN_RIGHT},
		{AF_PATTERN_BOTTOMLEFT,     AF_PATTERN_CENTER, AF_PATTERN_TOPLEFT,        AF_PATTERN_BOTTOM,         AF_PATTERN_LEFT,          AF_PATTERN_BOTTOMRIGHT},
		{AF_PATTERN_BOTTOMRIGHT,    AF_PATTERN_CENTER, AF_PATTERN_TOPRIGHT,       AF_PATTERN_BOTTOM,         AF_PATTERN_BOTTOMLEFT,    AF_PATTERN_RIGHT},

		{AF_PATTERN_LEFT,           AF_PATTERN_CENTER, AF_PATTERN_TOPLEFT,        AF_PATTERN_BOTTOMLEFT,     AF_PATTERN_LEFT,          AF_PATTERN_LEFTTRIANGLE},
		{AF_PATTERN_LEFTTRIANGLE,   AF_PATTERN_CENTER, AF_PATTERN_TOPTRIANGLE,    AF_PATTERN_BOTTOMTRIANGLE, AF_PATTERN_LEFT,          AF_PATTERN_LEFTDIAMOND},
		{AF_PATTERN_LEFTDIAMOND,    AF_PATTERN_CENTER, AF_PATTERN_TOPDIAMOND,     AF_PATTERN_BOTTOMDIAMOND,  AF_PATTERN_LEFTTRIANGLE,  AF_PATTERN_LEFTHALF},
		{AF_PATTERN_LEFTHALF,       AF_PATTERN_CENTER, AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTDIAMOND,   AF_PATTERN_VLINE},

		{AF_PATTERN_RIGHT,          AF_PATTERN_CENTER, AF_PATTERN_TOPRIGHT,       AF_PATTERN_BOTTOMRIGHT,    AF_PATTERN_RIGHTTRIANGLE, AF_PATTERN_RIGHT},
		{AF_PATTERN_RIGHTTRIANGLE,  AF_PATTERN_CENTER, AF_PATTERN_TOPTRIANGLE,    AF_PATTERN_BOTTOMTRIANGLE, AF_PATTERN_RIGHTDIAMOND,  AF_PATTERN_RIGHT},
		{AF_PATTERN_RIGHTDIAMOND,   AF_PATTERN_CENTER, AF_PATTERN_TOPDIAMOND,     AF_PATTERN_BOTTOMDIAMOND,  AF_PATTERN_RIGHTHALF,     AF_PATTERN_RIGHTTRIANGLE},
		{AF_PATTERN_RIGHTHALF,      AF_PATTERN_CENTER, AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_VLINE,         AF_PATTERN_RIGHTDIAMOND},

		{AF_PATTERN_HLINE,          AF_PATTERN_VLINE,  AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},
		{AF_PATTERN_VLINE,          AF_PATTERN_ALL,    AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},

		{AF_PATTERN_ALL,            AF_PATTERN_CENTER, AF_PATTERN_TOPHALF,        AF_PATTERN_BOTTOMHALF,     AF_PATTERN_LEFTHALF,      AF_PATTERN_RIGHTHALF},

		END_OF_LIST
};

int afp_transformer (int pattern, type_DIRECTION direction);

void afp_enter() {
	beep();
}

void afp_center () {
	send_to_intercom(IC_SET_AF_POINT, afp_transformer(DPData.af_point, DIRECTION_CENTER));
}

void afp_top () {
	send_to_intercom(IC_SET_AF_POINT, afp_transformer(DPData.af_point, DIRECTION_UP));
}

void afp_bottom () {
	send_to_intercom(IC_SET_AF_POINT, afp_transformer(DPData.af_point, DIRECTION_DOWN));
}

void afp_left () {
	send_to_intercom(IC_SET_AF_POINT, afp_transformer(DPData.af_point, DIRECTION_LEFT));
}

void afp_right () {
	send_to_intercom(IC_SET_AF_POINT, afp_transformer(DPData.af_point, DIRECTION_RIGHT));
}

int afp_transformer (int pattern, type_DIRECTION direction) {
	type_PATTERN_MAP_ITEM *item;

	// Loop over all items in the pattern map
	for (item = pattern_map; ! IS_EOL(item); item++) {

		// When we find an item matching the current pattern...
		if (item->pattern == pattern) {

			// ... we return the next pattern, according to the direction indicated
			switch (direction) {
			case DIRECTION_CENTER:
				return item->next_center;
			case DIRECTION_UP:
				return item->next_top;
			case DIRECTION_DOWN:
				return item->next_bottom;
			case DIRECTION_LEFT:
				return item->next_left;
			case DIRECTION_RIGHT:
				return item->next_right;
			}
		}
	}

	// Just in case something goes wrong
	return AF_PATTERN_CENTER;
}
