#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int main() {
    InitWindow(800, 450, "Typing Trainer - Improved UI");
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL));

    Font customFont = LoadFont("Robot-Regular.ttf");

    // ✅ Proper font load check
    if (customFont.baseSize == 0 || customFont.glyphCount == 0) {
        TraceLog(LOG_ERROR, "Failed to load custom font!");
        return 1;
    }

    Sound correctSound = LoadSound("correct.wav");

    const char* wordList[] = {
        "lol", "plo", "9l9", "ll", "l9", "po", "lo", "ol", "9p9", "pol",
        ".l.", ".o.", ".9.", "..o", "/;/", "/p/", "/0/", "0;0", "pool", "9/0"
    };
    const int wordCount = sizeof(wordList) / sizeof(wordList[0]);

    const char* target = wordList[rand() % wordCount];
    char input[32] = {0};
    int inputIndex = 0;

    bool showToast = false;
    double toastStart = 0.0;
    float toastY = -50;

    while (!WindowShouldClose()) {
        int key = GetCharPressed();
        if (key >= 32 && key <= 126 && inputIndex < (int)strlen(target)) {
            input[inputIndex++] = (char)key;
            input[inputIndex] = '\0';

            if (strcmp(input, target) == 0) {
                PlaySound(correctSound);
                showToast = true;
                toastStart = GetTime();
                toastY = -50;
                inputIndex = 0;
                input[0] = '\0';
                target = wordList[rand() % wordCount];
            } else if (inputIndex == strlen(target)) {
                inputIndex = 0;
                input[0] = '\0';
                target = wordList[rand() % wordCount];
            }
        }

        if (showToast) {
            if (toastY < 30) toastY += 4.0f;
            if (GetTime() - toastStart > 1.5) {
                toastY -= 4.0f;
                if (toastY < -50) showToast = false;
            }
        }

        BeginDrawing();

        // Gradient background
        for (int y = 0; y < 450; y++) {
            float t = (float)y / 450;
            Color c = ColorLerp(BLUE, RAYWHITE, t);
            DrawLine(0, y, 800, y, c);
        }

        // Font sizes
        int titleSize = 40;
        int labelSize = 24;
        int targetSize = 70;
        int inputSize = 60;
        int toastSize = 36;

        // Text: "Test Font!" (for preview)
        DrawTextEx(customFont, "Test Font!", (Vector2){100, 100}, titleSize, 1, BLACK);

        // Text: "Type this word:"
        Vector2 labelSizeVec = MeasureTextEx(customFont, "Type this word:", labelSize, 1);
        DrawTextEx(customFont, "Type this word:", 
                   (Vector2){400 - labelSizeVec.x / 2, 80}, labelSize, 1, DARKGRAY);

        // Target word
        Vector2 targetSizeVec = MeasureTextEx(customFont, target, targetSize, 1);
        DrawTextEx(customFont, target, 
                   (Vector2){400 - targetSizeVec.x / 2, 140}, targetSize, 1, DARKBLUE);

        // Input
        Vector2 inputSizeVec = MeasureTextEx(customFont, input, inputSize, 1);
        DrawTextEx(customFont, input, 
                   (Vector2){400 - inputSizeVec.x / 2, 260}, inputSize, 1, MAROON);

        // Toast
        if (showToast) {
            const char* message = "✅ Correct!";
            Vector2 msgSizeVec = MeasureTextEx(customFont, message, toastSize, 1);
            int x = 800 - (int)msgSizeVec.x - 40;
            float alpha = Clamp((toastY + 50) / 80, 0, 1);
            Color bgColor = Fade(GREEN, 0.25f * alpha);
            Color textColor = Fade(GREEN, alpha);

            DrawRectangle(x - 15, (int)toastY - 15, (int)msgSizeVec.x + 30, toastSize + 30, bgColor);
            DrawTextEx(customFont, message, (Vector2){x, (int)toastY}, toastSize, 1, textColor);
        }

        // Timer
        char timerText[64];
        snprintf(timerText, sizeof(timerText), "Time since start: %.1f seconds", GetTime());
        DrawTextEx(customFont, timerText, (Vector2){10, 10}, 20, 1, BLACK);

        EndDrawing();
    }

    UnloadSound(correctSound);
    UnloadFont(customFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

