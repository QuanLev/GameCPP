#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter(/* args */)
{
    texture = LoadTexture("characters/knight_idle_spritesheet.png");
    idle = LoadTexture("characters/knight_idle_spritesheet.png");
    run = LoadTexture("characters/knight_run_spritesheet.png");
}

BaseCharacter::~BaseCharacter()
{
}

Vector2 BaseCharacter::getWorldPos()
{
    return worldPos;
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{getScreenPos().x,
                    getScreenPos().y,
                    width * scale,
                    height * scale
                    };
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    // update animation frame
    runningTime += deltaTime;
    if (runningTime >= updateTime) {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrame) frame = 0;
    }
    
    float lenDirection = Vector2Length(velocity);
    if (lenDirection != 0.0) {
        // set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));

        if (velocity.x < 0.f) {
            rightLeft = -1.f;
        } else {
            rightLeft = 1.f;
        }

        texture = run;
    } else {
        texture = idle;
    }
    velocity = {};
    
    // draw the character
    Rectangle source{frame * width, 0.0, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, 4.0f * width, 4.0f * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.0, WHITE);
}