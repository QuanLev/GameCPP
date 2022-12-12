

#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"

class BaseCharacter
{
private:
    bool alive{true};

protected:
    Texture2D texture{};
    Texture2D idle{};
    Texture2D run{};
    //Vector2 screenPos{};
    // Position of the character
    Vector2 worldPos{};
    // 1: facing right, -1: facing left
    float rightLeft{-1.f};
    // animation variables
    float runningTime{};
    float updateTime{1.f / 12.f}; // our animation will update 12 times per second.
    int frame{};
    int maxFrame{6};
    float speed{4.f};
    const float scale{4.f};

    float width{};
    float height{};
    Vector2 worldPosLastFrame{};
    Vector2 velocity{};

public:
    BaseCharacter(/* args */);
    virtual ~BaseCharacter();
    Vector2 getWorldPos();
    void undoMovement();
    Rectangle getCollisionRec();
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;
    bool getAlive() {return alive;}
    void setAlive(bool isAlive) {alive = isAlive;}
};

#endif