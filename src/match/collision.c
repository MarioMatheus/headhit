#include "collision.h"

#define X 0
#define Y 1
#define WIDTH 2
#define HEIGHT 3

bool check_rect_collision (uint8_t* rect1, uint8_t* rect2) {
    return (
        rect1[X] < rect2[X] + rect2[WIDTH]
        && rect1[X] + rect1[WIDTH] > rect2[X]
        && rect1[Y] < rect2[Y] + rect2[HEIGHT]
        && rect1[Y] + rect1[HEIGHT] > rect2[Y]
    );
}
