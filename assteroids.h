#ifndef __ass
#define __ass

#include <raylib.h>
#include <cstdio>
#include <ctime>
int scrW = 800;
int scrH = 650;
Color Space = {21, 0, 26};
int astr_spawner = 0;
Vector4 nose, port, starboard, center;
float ship_bearing;
bool throttle = false;
bool ship_alive = true;

Vector4* bullets;
const int MAX_BULLETS = 32;
unsigned int score = 0;
Rectangle screenspace = {-200, -200, scrW+200, scrH+200};

// x/y pos, z heading, w size/life
Vector4* asteroids;
const int MAX_ASTEROIDS = 16;

Vector4* dead_ship;
const int SHIP_DEBRIS = 6;

Vector4 dead_astr[MAX_ASTEROIDS][6];
void explode_asteroid(Vector4* astr);
void update_explosions();
#endif //__ass

