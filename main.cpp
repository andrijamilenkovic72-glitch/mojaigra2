#include "raylib.h"
#include <math.h>

int main() {
    InitWindow(0, 0, "Moja Igra");
    SetTargetFPS(60);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float gravity = 0.6f;
    float pullPower = 0.15f;
    bool isGameActive = true;
    int level = 1;

    Vector2 playerPos = { screenWidth * 0.15f, screenHeight - 40.0f - 40.0f };
    Vector2 playerVel = { 0, 0 };
    float playerSize = 40.0f;
    bool isGrounded = true;

    Rectangle stub = { 0, 0, 50, 0 };
    bool isPulling = false;
    Vector2 startPoint = { 0, 0 };
    Vector2 currentPoint = { 0, 0 };
    bool wasPressed = false;

    auto InitLevel = [&]() {
        playerPos = { screenWidth * 0.15f, screenHeight - 40.0f - playerSize };
        playerVel = { 0, 0 };
        isGrounded = true;

        float stubHeight = (screenHeight * 0.15f) + (level * (screenHeight * 0.03f));
        if (stubHeight > screenHeight * 0.6f) stubHeight = screenHeight * 0.6f;
        stub = { screenWidth * 0.55f, screenHeight - 40.0f - stubHeight, 50, stubHeight };
    };

    InitLevel();

    while (!WindowShouldClose()) {
        Vector2 inputPos = GetTouchPosition(0);
        bool isPressed = GetTouchPointCount() > 0;

        if (isGameActive) {
            if (isPressed && !wasPressed) {
                if (isGrounded && CheckCollisionPointCircle(inputPos, playerPos, 150)) {
                    isPulling = true;
                    startPoint = inputPos;
                }
            }

            if (isPulling) {
                currentPoint = inputPos;
                if (!isPressed) {
                    isPulling = false;
                    Vector2 pullVector = { startPoint.x - currentPoint.x, startPoint.y - currentPoint.y };
                    if (sqrt(pullVector.x * pullVector.x + pullVector.y * pullVector.y) > 20) {
                        isGrounded = false;
                        playerVel.x = pullVector.x * pullPower;
                        playerVel.y = pullVector.y * pullPower;
                    }
                }
            }
            wasPressed = isPressed;

            if (!isGrounded) {
                playerVel.y += gravity;
                playerVel.x *= 0.99f;
                playerPos.x += playerVel.x;
                playerPos.y += playerVel.y;
            }

            float floorY = screenHeight - 40;

            if (!isGrounded && playerPos.y + playerSize >= floorY) {
                playerPos.y = floorY - playerSize;
                playerVel = { 0, 0 };
                isGrounded = true;
                isGameActive = false;
            }

            Rectangle playerRect = { playerPos.x, playerPos.y, playerSize, playerSize };
            if (CheckCollisionRecs(playerRect, stub)) {
                if (playerPos.y + playerSize >= stub.y && playerPos.y + playerSize <= stub.y + 15) {
                    float overlap = fmin(playerPos.x + playerSize, stub.x + stub.width) - fmax(playerPos.x, stub.x);
                    if (overlap > playerSize * 0.5f) {
                        playerPos.y = stub.y - playerSize;
                        playerVel = { 0, 0 };
                        isGrounded = true;
                        level++;
                        InitLevel();
                    } else { isGameActive = false; }
                } else if (playerPos.y + playerSize > stub.y + 15) { isGameActive = false; }
            }
        } else {
            if (isPressed) {
                level = 1;
                InitLevel();
                isGameActive = true;
            }
        }

        BeginDrawing();
        ClearBackground({ 20, 20, 30, 255 });

        if (isGameActive) {
            DrawRectangle(0, screenHeight - 40, screenWidth, 40, BLACK);
            DrawRectangleRec(stub, LIGHTGRAY);
            DrawText(TextFormat("%d", level), stub.x + 10, stub.y - 30, 24, WHITE);
            DrawRectangleV(playerPos, { playerSize, playerSize }, WHITE);
        } else {
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            DrawText("GAME OVER", centerX - 90, centerY - 40, 40, RED);
            DrawText(TextFormat("Level: %d", level - 1), centerX - 50, centerY + 10, 20, WHITE);
            DrawText("Tap to restart", centerX - 70, centerY + 50, 20, GRAY);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
