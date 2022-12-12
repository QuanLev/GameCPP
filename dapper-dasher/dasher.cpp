#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    int vel;
};

bool isOnGround(AnimData data, int wHeight) {
    return (data.pos.y >= (wHeight - data.rec.height));
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame) {
    data.runningTime += deltaTime;
    
    // update nebula animation frame
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame) {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    int windowDimensions[2]{};
    windowDimensions[0] = 850;
    windowDimensions[1] = 550;
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper - Dasher");

    int velocity{0};
    // acceleration due to gravity (pixels/s)/s
    int gravity{1'000};
    bool isInAir = false;
    int jumpVel{-600}; // pixels/second

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    int sizeAnimData{5};
    AnimData neb[sizeAnimData]{};
    for (int i = 0; i < sizeAnimData; ++i) {
        neb[i].rec.x = 0;
        neb[i].rec.y = 0;
        neb[i].rec.width = nebula.width/8;
        neb[i].rec.height = nebula.height/8;

        neb[i].pos.x = windowDimensions[0] + (i * 300);
        neb[i].pos.y = windowDimensions[1] - neb[i].rec.height;
        neb[i].frame = 0;
        neb[i].updateTime = 1.0/12.0;
        neb[i].runningTime = 0.0;
        neb[i].vel = -200;
    }

    float finishLine{neb[sizeAnimData - 1].pos.x};

    // scarfy variavles
    Texture2D scarf = LoadTexture("textures/scarf.png");
    AnimData scarfData{{0.0, 0.0, scarf.width/6, scarf.height}, {windowDimensions[0]/2 - scarf.width/12, windowDimensions[1] - scarf.height}, 0, 1.0/12.0, 0.0, -600};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    float mgX{};
    float fgX{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        float dT = GetFrameTime();  // Delta time

        // draw the background
        bgX -= 20 * dT;
        if (bgX <= -background.width *3.5) {
            bgX = 0.0;
        }
        
        mgX -= 40 * dT;
        if (mgX <= -midground.width *3.5) {
            mgX = 0.0;
        }

        fgX -= 80 * dT;
        if (fgX <= -foreground.width *3.5) {
            fgX = 0.0;
        }

        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.5, WHITE);
        Vector2 bg2Pos{bgX + background.width * 3.5, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 3.5, WHITE);

        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.5, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 3.5, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 3.5, WHITE);

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.5, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 3.5, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.5, WHITE);

        //if (scarfData.pos.y >= windowDimensions[1] - scarfData.rec.height) {
        if (isOnGround(scarfData, windowDimensions[1])) {
            velocity = 0;
            isInAir = false;
        } else {
            velocity += gravity * dT;
            isInAir = true;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (!isInAir) velocity += jumpVel;
        }

        bool collision{};
        for (AnimData nebula : neb) {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };

            Rectangle scarfRec{
                scarfData.pos.x,
                scarfData.pos.y,
                scarfData.rec.width,                
                scarfData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfRec)) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            DrawText("Game Over", scarfData.pos.x, scarfData.pos.y, 55, RED);
        } else if (scarfData.pos.x >= finishLine) {
            DrawText("You Win", scarfData.pos.x, scarfData.pos.y, 55, YELLOW);
        } else {
            // for nebula
            for (int i = 0; i < sizeAnimData; ++i) {
                // update the nebula position
                neb[i].pos.x += neb[i].vel * dT;
                neb[i] = updateAnimData(neb[i], dT, 7);
                DrawTextureRec(nebula, neb[i].rec, neb[i].pos, WHITE);
            }

            // update finish line
            finishLine += neb[0].vel * dT;

            // update the scarfy position
            scarfData.pos.y += velocity * dT;
            // update scarfy animation frame
            if (!isInAir) {
                // for scarfy
                scarfData = updateAnimData(scarfData, dT, 5);
            }

            // draw scarfy
            DrawTextureRec(scarf, scarfData.rec, scarfData.pos, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(scarf);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}