#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Typing Trainer - Auto Next w/ Toast");
	InitAudioDevice();
	SetTargetFPS(60);
	srand(time(NULL));

	Sound correctSound = LoadSound("correct.wav");

	const char* wordList[] = {
		"lol", "plo", "9l9", "ll", "l9", "po", "lo", "ol", "9p9", "pol"
	};
	const int wordCount = sizeof(wordList) / sizeof(wordList[0]);

	const char* target = wordList[rand() % wordCount];
	char input[32] = {0};
	int inputIndex = 0;

	// Toast message state
	bool showToast = false;
	double toastStart = 0.0;

	while (!WindowShouldClose()) {
		// Typing
		int key = GetCharPressed();
		if (key >= 32 && key <= 126 && inputIndex < (int)strlen(target)) {
			input[inputIndex++] = (char)key;
			input[inputIndex] = '\0';

			if (strcmp(input, target) == 0) {
				PlaySound(correctSound);

				// Show toast
				showToast = true;
				toastStart = GetTime();

				// Load next word
				inputIndex = 0;
				input[0] = '\0';
				target = wordList[rand() % wordCount];
			}
			else if (inputIndex == strlen(target))
			{
				// Load next word
				inputIndex = 0;
				input[0] = '\0';
				target = wordList[rand() % wordCount];

			}
		}

		// Hide toast after 1.5s
		if (showToast && (GetTime() - toastStart > 1.5)) {
			showToast = false;
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawText("Type this word:", 100, 80, 20, GRAY);
		DrawText(target, 100, 120, 50, DARKBLUE);

		DrawText("Your input:", 100, 200, 20, GRAY);
		DrawText(input, 100, 240, 50, MAROON);

		if (showToast) {
			int fontSize = 30;
			const char* message = "✅ Correct!";
			int textWidth = MeasureText(message, fontSize);
			int x = screenWidth - textWidth - 30;
			int y = 30;

			// Optional background box
			DrawRectangle(x - 10, y - 10, textWidth + 20, fontSize + 20, Fade(GREEN, 0.2f));
			DrawText(message, x, y, fontSize, GREEN);
        }

        EndDrawing();
    }

    UnloadSound(correctSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
