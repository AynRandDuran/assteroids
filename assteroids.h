#ifndef __ass
#define __ass

#include <raylib.h>
#include <cstdio>
#include <ctime>

#define SFX_SHOOT_FILE "./shoot.wav"
#define SFX_BGM_FILE "./music.wav"

Sound sfx_music;
Sound sfx_shoot;
bool paused = false;
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
time_t last_fire = 0;

// x/y pos, z heading, w size/life
Vector4* asteroids;
const int MAX_ASTEROIDS = 16;

Vector4* dead_ship;
const int SHIP_DEBRIS = 6;

Vector4 dead_astr[MAX_ASTEROIDS][6];
void explode_asteroid(Vector4* astr);
void update_explosions();

// bitfield
const int SHOTGUN = 1;
const int BOMB = 2;
const int GOD = 4;
char active_powerups = 0;
int shotgun_blasts = 0;
int bullet_offsets[5] = {0, 6, 12, -6, -12};

Vector4 shotgun_box = {center.x, center.y, 45, 0};
Vector2 master_s_top_box[4] = {{-10, -2}, {-10, -10}, {10, -10}, {10, -2}};
Vector2 master_s_bot_box[4] = {{-10, 2}, {-10, 10}, {10, 10}, {10, 2}};
Vector2 s_top_box[4];
Vector2 s_bot_box[4];
void init_shotgun();
void draw_shotgun();
int enable_shotgun();
int disable_shotgun();

Vector4 shield_pickup = {0, 0, 0, 0};
int shield_hp = 9;
time_t shield_spawn_time = 0;
void init_shield();
void draw_shield_pickup(); //explicitly, the powerup
void enable_shield();
void disable_shield();

Vector4 bomb_proj;
int whrot = 0;
void launch_bomb();
void update_bomb();

// Death stats
int bomb_kills     = 0;
int shotgun_kills  = 0;
int deaths_avoided = 0;
int time_alive     = 0;

#endif //__ass

