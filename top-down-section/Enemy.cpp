#include "Enemy.h"
#include "raymath.h"


Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = texture.width / maxFrame;
    height = texture.height;
    speed = 2.8f;
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive()) return;
    // get toTarget                         Enemy -     character
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    // When an enemy close enough to the character, so that enemy stop chasing.
    if (Vector2Length(velocity) < radius) {
        velocity = {};
    }
    BaseCharacter::tick(deltaTime);
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}

void Enemy::setTarget(Character* tg)
{
    if (tg) {
        target = tg;
    }
}