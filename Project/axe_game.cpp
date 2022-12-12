#include "raylib.h"

int main() 
{
    int width = 700;
    int height = 500;
    InitWindow(width, height, "First Windows");
    
    int circle_x = 100;
    int circle_y = 100;
    int axe_x = 300;
    int axe_y = 0;
    int direction = 10;
    int circle_radius = 50;
    int axe_length = 50;
    int l_circle_x = circle_x - circle_radius;
    int r_circle_x = circle_x + circle_radius;
    int u_circle_y = circle_y - circle_radius;
    int b_circle_y = circle_y + circle_radius;
    int l_axe_x = axe_x;
    int r_axe_x = axe_x + axe_length;
    int u_axe_y = axe_y;
    int b_axe_y = axe_y + axe_length;
    bool collision_with_axe = b_axe_y >= u_circle_y &&
                b_circle_y >= u_axe_y &&
                r_circle_x >= l_axe_x &&
                r_axe_x >= l_circle_x;

    SetTargetFPS(60);
    while(WindowShouldClose() == false) {
            BeginDrawing();
            ClearBackground(BLUE);
        if (collision_with_axe) {
            DrawText("Game Over", 300, 200, 20, RED);
        } else {
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);
            axe_y += direction;
            if (axe_y > height || axe_y < 0) {
                direction = -direction;
            }
            if (IsKeyDown(KEY_D)) {
                if (circle_x < (height - circle_radius)) {
                    circle_x += 10;
                }
            }
            if (IsKeyDown(KEY_A)) {
                if (circle_x > circle_radius) {
                    circle_x -= 10;
                }
            }
            DrawCircle(circle_x, circle_y, circle_radius, YELLOW);
            
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            b_circle_y = circle_y + circle_radius;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_length;

            collision_with_axe = b_axe_y >= u_circle_y &&
                                b_circle_y >= u_axe_y &&
                                r_circle_x >= l_axe_x &&
                                r_axe_x >= l_circle_x;
        }

        EndDrawing();
    };
}