#include "assteroids.h"
#include <cstring>
#include <math.h>
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
    
    nose.x = 0; nose.y = -15;
    port.x = -10; port.y = 15;
    starboard.x = 10; starboard.y = 15;
    center.x = scrW/2; center.y = scrH/2;
    ship_bearing = 0;
    
    memset(bullets, 0, sizeof(Vector4) * MAX_BULLETS);
    memset(asteroids, 0, sizeof(Vector4) * MAX_ASTEROIDS);
    memset(dead_ship, 0, sizeof(Vector4) * SHIP_DEBRIS);    

    memset(dead_astr, 0, sizeof(Vector4) * SHIP_DEBRIS * MAX_ASTEROIDS);
}

void die(){
    ship_alive = false;
    for(int i = 0; i < SHIP_DEBRIS; i++) {
        if (!onscreen(flatten(dead_ship[i])))
            continue;
        dead_ship[i].x = center.x; dead_ship[i].y = center.y;
        dead_ship[i].z = (360/SHIP_DEBRIS) * (i + 1);
        dead_ship[i].w = 4;
    }
    
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
    for(int i = 0; i < MAX_BULLETS; i++){
        // find available bullet
        if(bullets[i].w == 0) {
            bullets[i].x = center.x;
            bullets[i].y = center.y;
            bullets[i].z = nose.z;
            bullets[i].w = 1; 
            break;
        }
    }
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

        if(ship_collision(astr) && ship_alive){
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

        if(IsKeyDown('J') && ship_alive) {
            spin_ship(-4);
        }
        if(IsKeyDown('L') && ship_alive) {
            spin_ship(4);
        }
        if(IsKeyPressed('I') && ship_alive) {
            throttle = true;
        }
        if(IsKeyPressed('S') && ship_alive) {
            shoot();
        }
        if(IsKeyPressed('R') && !ship_alive) {
            init_ship();
            spin_ship(0);
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
        } else {
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

void explode_asteroid(Vector4* astr) {
    for(int i = 0; i < MAX_ASTEROIDS; i++) {
        if(dead_astr[i][0].w == 0) {
            for (int a = 0; a < SHIP_DEBRIS; a++) {
                dead_astr[i][a].x = astr->x;
                dead_astr[i][a].y = astr->y;
                dead_astr[i][a].z = ((360/SHIP_DEBRIS) * a) + (360/(SHIP_DEBRIS*3));
                dead_astr[i][a].w = 1;
            }
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
