#ifndef __ass
#define __ass

#include <raylib.h>
#include <cstdio>
#include <ctime>
int scrW = 800;
int scrH = 650;

Color Space = {21, 0, 26};
Vector4 nose, port, starboard, center;
float ship_bearing;
bool throttle = false;
bool ship_alive = true;

Vector4* bullets;
const int MAX_BULLETS = 32;
unsigned int score = 0;
Rectangle screenspace = {-10, -10, scrW+10, scrH+10};

// x/y pos, z heading, w size/life
Vector4* asteroids;
const int MAX_ASTEROIDS = 16;
#endif //__ass

