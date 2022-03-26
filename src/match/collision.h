#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <gb/gb.h>
#include <stdbool.h>

/**
 * Returns TRUE if exists collision between rects
 * uint8_t* rect {x, y, width, height}
 */
bool check_rect_collision (uint8_t* rect1, uint8_t* rect2);

#endif
