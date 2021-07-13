#include <cstring>
#include <math.h>
#include "assteroids.h"
#include "powerups.hpp"
#include "vectormath.hpp"

Vector2 flatten(Vector4 pV){
    Vector2 stan = {pV.x, pV.y};
    return stan;
}

Vector4 translate(Vector4 v1, Vector4 v2) {
    Vector4 translation = {v1.x+v2.x, v1.y+v2.y, v1.y+v2.y, v1.z+v2.z};
    return translation;
}

bool onscreen(Vector2 v) {
    // Ensure v is still onscreen
    return CheckCollisionPointRec(v, screenspace); 
}

void init_ship(){
    ship_alive = true;
    throttle = false;
    score = 0;
    nose.z = 270;
    port.z = 55;
    starboard.z = 125;
    active_powerups = 0;
    
    nose.x = 0; nose.y = -15;
    port.x = -10; port.y = 15;
    starboard.x = 10; starboard.y = 15;
    center.x = scrW/2; center.y = scrH/2;
    ship_bearing = 0;
    
    memset(bullets, 0, sizeof(Vector4) * MAX_BULLETS);
    memset(asteroids, 0, sizeof(Vector4) * MAX_ASTEROIDS);
    memset(dead_ship, 0, sizeof(Vector4) * SHIP_DEBRIS);    
    init_shotgun();
    memset(&shotgun_box, 0, sizeof(Vector4));
    memset(dead_astr, 0, sizeof(Vector4) * SHIP_DEBRIS * MAX_ASTEROIDS);
    memset(&shield_pickup, 0, sizeof(Vector4));
    memset(&bomb_proj, 0, sizeof(Vector4));
}

void die(){
    ship_alive = false;
    disable_shield();
    for(int i = 0; i < SHIP_DEBRIS; i++) {
        if (!onscreen(flatten(dead_ship[i])))
            continue;
        dead_ship[i].x = center.x; dead_ship[i].y = center.y;
        dead_ship[i].z = (360/SHIP_DEBRIS) * (i + 1);
        dead_ship[i].w = 6;
    }
    disable_shotgun();
    
}

void spin_ship(int az_delta){
    nose.z += az_delta; port.z += az_delta; starboard.z += az_delta;
    nose.x = cos(nose.z * (PI/180)) * 15;
    nose.y = sin(nose.z * (PI/180)) * 15;

    port.x = cos(port.z * (PI/180)) * 15;
    port.y = sin(port.z * (PI/180)) * 15;
    
    starboard.x = cos(starboard.z * (PI/180)) * 15;
    starboard.y = sin(starboard.z * (PI/180)) * 15;

}

void update_v4(Vector4* origin, Vector4* v_body, int speed){
    Vector2 heading = v2_normal(flatten(*origin));
    v_body->x += heading.x * (speed*throttle); 
    v_body->y += heading.y * (speed*throttle);
}

void shoot(){
    if(time(NULL) <= last_fire)
        return;
    for(int s = 0; s < 1 + (4 * (active_powerups & SHOTGUN)); s++) {
        for(int i = 0; i < MAX_BULLETS; i++){
            // find available bullet
            if(bullets[i].w == 0) {
                bullets[i].x = center.x;
                bullets[i].y = center.y;
                bullets[i].z = nose.z + bullet_offsets[s];
                bullets[i].w = 1; 
                break;
            }
        }
    }
    if(active_powerups & SHOTGUN)
        shotgun_blasts--;
    if(shotgun_blasts == 0 && active_powerups & SHOTGUN)
        disable_shotgun();
    time(&last_fire);
}

void update_bullets(){
    Vector4* b;
    for(int i = 0; i < MAX_BULLETS; i++){
        if(!onscreen(flatten(bullets[i])))
            bullets[i].w = 0;
        
        if(bullets[i].w == 1) {
            b = &bullets[i];
            b->x += cos(b->z * (PI/180)) * 15;
            b->y += sin(b->z * (PI/180)) * 15;
            DrawCircleV(flatten(bullets[i]), 2, WHITE);
            for(int a = 0; a < MAX_ASTEROIDS; a++) {
                if(CheckCollisionPointCircle(flatten(bullets[i]), flatten(asteroids[a]), asteroids[a].w)) {
                    asteroids[a].w = 0;
                    bullets[i].w = 0;
                    score++;
                    explode_asteroid(&asteroids[a]);
                    break;
                }
            } 
        }
    }
}

void spawn_astr() {
    int heading = rand() % 360;
    for(int i = 0; i < MAX_ASTEROIDS; i++){
        // find available asteroid
        // no size means you're fuckin dead, gyro
        if(asteroids[i].w == 0) {
            asteroids[i].z = heading; 
            asteroids[i].w = rand() % 34 + 30; 
            asteroids[i].x = (heading > 90 && heading < 270) ? scrW : 0; 
            asteroids[i].y = (heading > 180 && heading < 360) ? scrH : 0; 
            break;
        }
    }
}

void explode_ship() {
    for (int i = 0; i < SHIP_DEBRIS; i++) {
        DrawCircleLines(dead_ship[i].x, dead_ship[i].y, dead_ship[i].w, RED);
        dead_ship[i].x += cos(dead_ship[i].z * (PI/180)) *3;
        dead_ship[i].y += sin(dead_ship[i].z * (PI/180)) *3;
    }
}

bool ship_collision(Vector4* body) {
    return (
            CheckCollisionPointCircle(flatten(translate(nose, center)), flatten(*body), body->w * 0.9f) ||
            CheckCollisionPointCircle(flatten(translate(port, center)), flatten(*body), body->w * 0.9f) ||
            CheckCollisionPointCircle(flatten(translate(starboard, center)), flatten(*body), body->w * 0.9f)
            );

}

void update_astrs() {
    Vector4* astr;
    for(int i = 0; i < MAX_ASTEROIDS; i++) {
        astr = &asteroids[i];
        if(astr->w != 0) {
            astr->x += cos(astr->z * (PI/180)) *3;
            astr->y += sin(astr->z * (PI/180)) *3;
            DrawPolyLines(flatten(*astr), 7, astr->w, 0, RAYWHITE);
        }
        if(!onscreen(flatten(*astr))) {
            astr->w = 0;
        }
        if(CheckCollisionCircles(flatten(*astr), astr->w, flatten(bomb_proj), bomb_proj.w) && bomb_proj.w >= 16) {
            astr->w = 0;
            bomb_proj.z = -1; //stop moving the bomb and explode
            bomb_proj.w +=16;
            explode_asteroid(astr);
        }

        if(ship_collision(astr) && ship_alive){
            if((active_powerups & GOD)) {
                continue;
            }
            die();
        }
    }

}

int main(void) {
    asteroids = (Vector4*)malloc(sizeof(Vector4)*MAX_ASTEROIDS);
    bullets = (Vector4*)malloc(sizeof(Vector4)*MAX_BULLETS);
    dead_ship = (Vector4*)malloc(sizeof(Vector4) * SHIP_DEBRIS);

    srand(time(NULL));
    InitWindow(scrW, scrH, "Assteroids Raylib");
    SetTargetFPS(50);
    init_ship();
    spin_ship(0);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Space);

        if((IsKeyDown('J') || IsKeyDown(KEY_LEFT)) && ship_alive) {
            spin_ship(-4);
        }
        if((IsKeyDown('L') || IsKeyDown(KEY_RIGHT)) && ship_alive) {
            spin_ship(4);
        }
        if((IsKeyPressed('I') || IsKeyPressed(KEY_UP)) && ship_alive) {
            throttle = true;
        }
        if(IsKeyPressed('S') && ship_alive) {
            shoot();
        }
        if(IsKeyPressed('R') && !ship_alive) {
            init_ship();
            spin_ship(0);
        }
        if(IsKeyPressed('A') && (active_powerups & BOMB)) {
            active_powerups ^= BOMB;
            launch_bomb();
        }

        if(!onscreen(flatten(center)) && ship_alive)
            die();

        if(ship_alive) {
            update_v4(&nose, &center, 3);
            DrawTriangleLines(
                flatten(translate(nose, center)),
                flatten(translate(port, center)),
                flatten(translate(starboard, center)),
                RAYWHITE);
            astr_spawner = rand() & 1000 + 1;
            if(active_powerups & GOD) {
                DrawCircleSectorLines(flatten(center), 3*shield_hp, 0, 360, 6, SKYBLUE);
                DrawCircleSectorLines(flatten(center), 2*shield_hp, 0, 360, 6, SKYBLUE);
                if(time(NULL) > shield_spawn_time) {
                    time(&shield_spawn_time);
                    shield_hp--;
                }
                if(shield_hp <= 0)
                    disable_shield();
            }
            if(bomb_proj.w != 0)
                update_bomb();
            if(!throttle) {
                DrawText("KILL TO LIVE", (scrW/2)-(MeasureText("KILL TO LIVE", 64)/2), (scrH/2)-128, 64, RED);
                DrawText("LAUNCH TO START", (scrW/2)-(MeasureText("LAUNCH TO START", 32)/2), (scrH/2)+64, 32, RED);
            }
            if(score%5 == 0 && score > 0 && bomb_proj.w < 16) {
                //Bomb unlocked
                active_powerups ^= BOMB;
            }
            if(active_powerups & BOMB) 
                DrawCircleSectorLines(flatten(translate(nose, center)), 4.0f, 0, 360, 360, RED);
        } else if(!(active_powerups & GOD)) {
            DrawText("YOU DIED", (scrW/2)-(MeasureText("YOU DIED", 64)/2), (scrH/2)-64, 64, RED);
            DrawText("FUCK YOU", (scrW/2)-(MeasureText("FUCK YOU", 16)/2), (scrH/2)-8, 16, RED);
            explode_ship();
        }
        if(throttle && astr_spawner > 989) {
            spawn_astr();
        }
        update_bullets();
        update_astrs();
        update_explosions();
        if(throttle && shotgun_box.w == 0 && (rand() % 10000) < 8 && !active_powerups & SHOTGUN)
            init_shotgun(); //a little treat
        if(shotgun_box.w == 1)
            draw_shotgun();
        if(shield_pickup.w > 0)
            draw_shield_pickup();
        
        DrawText(TextFormat("%d", score), 4, scrH-34, 32, WHITE);
        DrawFPS(0, 0);
        EndDrawing();

    }
    CloseWindow();
    free(bullets);
    free(asteroids);
    free(dead_ship);
    
    return 0;
}

//spawn a shield pickup at a dead asteroid
void init_shield(Vector4* v) {
    shield_pickup.x = v->x;
    shield_pickup.y = v->y;
    shield_pickup.w = 16;
}
void draw_shield_pickup() {
    DrawRingLines(flatten(shield_pickup), shield_pickup.w, shield_pickup.w * 1.3, 0, 360, 6, SKYBLUE);
    DrawRingLines(flatten(shield_pickup), shield_pickup.w/3, shield_pickup.w * 0.66, 0, 360, 6, SKYBLUE);
    if(CheckCollisionPointCircle(flatten(center), flatten(shield_pickup), shield_pickup.w)) {
        shield_pickup.w = 0;
        enable_shield();
    }
}

void enable_shield() {
    memset(&shield_pickup, 0, sizeof(Vector4));
    shield_hp = 9;
    active_powerups |= GOD;
    time(&shield_spawn_time);
}

void disable_shield() {
    active_powerups ^= GOD;
    shield_hp = 9;
}

void explode_asteroid(Vector4* astr) {
    for(int i = 0; i < MAX_ASTEROIDS; i++) {
        if(dead_astr[i][0].w == 0) {
            for (int a = 0; a < SHIP_DEBRIS; a++) {
                dead_astr[i][a].x = astr->x;
                dead_astr[i][a].y = astr->y;
                dead_astr[i][a].z = ((360/SHIP_DEBRIS) * a) + (360/(SHIP_DEBRIS*3));
                dead_astr[i][a].w = 1;
            }
            if(shield_pickup.w == 0 && rand() % 30 < 2)
                init_shield(astr);
            break;
        }
    }
}

void update_explosions() {
    int offscreen_bubbles = 0;
    for(int i = 0; i < MAX_ASTEROIDS; i++) {
        if(dead_astr[i][0].w == 1) {
            for (int a = 0; a < SHIP_DEBRIS; a++) {
                dead_astr[i][a].x += cos(dead_astr[i][a].z * (PI/180)) * 5;
                dead_astr[i][a].y += sin(dead_astr[i][a].z * (PI/180)) * 5;
                DrawCircleLines(dead_astr[i][a].x, dead_astr[i][a].y, 8, RAYWHITE);
                if(!onscreen(flatten(dead_astr[i][a])))
                    offscreen_bubbles++;
                if(offscreen_bubbles == SHIP_DEBRIS-1)
                    memset(&dead_astr[i], 0, sizeof(Vector4) * SHIP_DEBRIS);
            }
        }
        offscreen_bubbles=0;
    }
    
}

void init_shotgun() {
    int heading = rand() % 360;
    shotgun_box.z = heading; 
    shotgun_box.w = 1; 
    shotgun_box.x = (heading > 90 && heading < 270) ? scrW : 0; 
    shotgun_box.y = (heading > 180 && heading < 360) ? scrH : 0; 

    memcpy(s_top_box, master_s_top_box, sizeof(Vector2)*4); memcpy(s_bot_box, master_s_bot_box, sizeof(Vector2)*4);
    for(int i = 0; i < 4; i++) {
        s_top_box[i].x += shotgun_box.x;
        s_top_box[i].y += shotgun_box.y;
        s_bot_box[i].x += shotgun_box.x;
        s_bot_box[i].y += shotgun_box.y;
    }
}

void update_shotgun() {
    float xd = cos(shotgun_box.z * (PI/180)) *2;
    float yd = sin(shotgun_box.z * (PI/180)) *2;
    shotgun_box.x += xd;
    shotgun_box.y += yd;

    for(int i = 0; i < 4; i++) {
        s_top_box[i].x += xd;
        s_top_box[i].y += yd;
        s_bot_box[i].x += xd;
        s_bot_box[i].y += yd;
    }

    if(!onscreen(flatten(shotgun_box)))
        shotgun_box.w = 0;
    if(CheckCollisionPointCircle(flatten(center), flatten(shotgun_box), 32)) {
        enable_shotgun();
        shotgun_box.w = 0;
    }
}

void draw_shotgun() {
    update_shotgun();
    DrawLineStrip(s_top_box, 4, WHITE);
    DrawLineStrip(s_bot_box, 4, WHITE);
    DrawText("S", shotgun_box.x-6, shotgun_box.y-8, 20, GOLD);
}

void launch_bomb() {
    bomb_proj.x = flatten(translate(nose, center)).x;
    bomb_proj.y = flatten(translate(nose, center)).y;
    bomb_proj.z = nose.z; 
    bomb_proj.w = 16;
}

void update_bomb() {
    if(bomb_proj.z != -1) {
        bomb_proj.x += cos(bomb_proj.z * (PI/180)) *6;
        bomb_proj.y += sin(bomb_proj.z * (PI/180)) *6;
    }
    if(bomb_proj.w < 128) {
        DrawPolyLines(flatten(bomb_proj), 8, bomb_proj.w, whrot, DARKPURPLE);
        whrot++;
    } else if (bomb_proj.w >= 128 || !onscreen(flatten(bomb_proj))) {
        memset(&bomb_proj, 0, sizeof(Vector4));
    }
}
