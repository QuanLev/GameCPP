#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main() {

    int windowWidth{384};
    int windowHeight{384};


    InitWindow(windowWidth, windowHeight, "Stephen's Top Down");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0};
    Character knight(windowWidth, windowHeight);
    Prop rock{Vector2{0.f, 0.f}, LoadTexture("nature_tileset/Rock.png")};
    Prop props[2]{{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
                  {Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}};

    Enemy goblin{Vector2{800.f, 300.f}, 
                        LoadTexture("characters/goblin_idle_spritesheet.png"), 
                        LoadTexture("characters/goblin_run_spritesheet.png")};
    Enemy slime{Vector2{500.f, 700.f}, 
                        LoadTexture("characters/slime_idle_spritesheet.png"), 
                        LoadTexture("characters/slime_run_spritesheet.png")};
    Enemy* enemies[]{
        &goblin,
        &slime
    };
    for (auto e : enemies) {
        e->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        knight.tick(GetFrameTime());
        for (Prop prop : props) {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive()) {
            DrawText("Game Over", 55.f, 50.f, 45, RED);
            EndDrawing();
            continue;
        } else {
            std::string knightHelth = "Health: ";
            knightHelth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightHelth.c_str(), 55.f, 50.f, 45, RED);
        }

        if (knight.getWorldPos().x < 0 ||
            knight.getWorldPos().y < 0 ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        for (Prop prop : props) {
            bool isCollision = CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec());
            if (isCollision) knight.undoMovement();
        }

        goblin.tick(GetFrameTime());
        for (auto e : enemies) {
            e->tick(GetFrameTime());
        }

        for (auto e : enemies) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                bool isCollision = CheckCollisionRecs(e->getCollisionRec(), knight.getWeaponCollisionRec());
                if (isCollision) e->setAlive(false);
            }
        }

        EndDrawing();
    }
    CloseWindow();
}