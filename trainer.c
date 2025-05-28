#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

static float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

int main() {
	Font customFont = LoadFont("Brunson.ttf");
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Typing Trainer - Improved UI");
	InitAudioDevice();
	SetTargetFPS(60);
	srand(time(NULL));

	Sound correctSound = LoadSound("correct.wav");

	const char* wordList[] = {
		"lol", "plo", "9l9", "ll", "l9", "po", "lo", "ol", "9p9", "pol", ".l.", ".o.", ".9.", "..o", "/;/", "/p/", "/0/", "0;0", "pool", "9/0"
	};
	const int wordCount = sizeof(wordList) / sizeof(wordList[0]);

	const char* target = wordList[rand() % wordCount];
	char input[32] = {0};
	int inputIndex = 0;

	// Toast message state
	bool showToast = false;
	double toastStart = 0.0;
	float toastY = -50;   // Start above screen for slide effect
	while (!WindowShouldClose()) {
		int key = GetCharPressed();
		if (key >= 32 && key <= 126 && inputIndex < (int)strlen(target)) {
			input[inputIndex++] = (char)key;
			input[inputIndex] = '\0';

			if (strcmp(input, target) == 0) {
				PlaySound(correctSound);

				// Show toast
				showToast = true;
				toastStart = GetTime();
				toastY = -50; // Reset position for slide down

				// Load next word
				inputIndex = 0;
				input[0] = '\0';
				target = wordList[rand() % wordCount];
			} else if (inputIndex == strlen(target)) {
				// Load next word even if wrong
				inputIndex = 0;
				input[0] = '\0';
				target = wordList[rand() % wordCount];
			}
		}

		// Update toast animation
		if (showToast) {
			// Slide down toast from top
			if (toastY < 30) toastY += 4.0f;

			// Hide toast after 1.5s
			if (GetTime() - toastStart > 1.5) {
				// Slide up before hiding
				toastY -= 4.0f;
				if (toastY < -50) showToast = false;
			}
		}
		BeginDrawing();

		// Gradient background (top blue to bottom white)
		for (int y = 0; y < screenHeight; y++) {
			float t = (float)y / screenHeight;
			Color c = ColorLerp(BLUE, RAYWHITE, t);
			DrawLine(0, y, screenWidth, y, c);
		}

		// Example custom text
		DrawTextEx(customFont, "Test Font!", (Vector2){100, 100}, 40, 1, BLACK);

		// Draw instructions
		Vector2 instructSize = MeasureTextEx(customFont, "Type this word:", 24, 1);
		DrawTextEx(customFont, "Type this word:", (Vector2){
				screenWidth / 2 - instructSize.x / 2, 80
				}, 24, 1, DARKGRAY);

		// Draw target word (centered, bigger)
		Vector2 targetSize = MeasureTextEx(customFont, target, 70, 1);
		DrawTextEx(customFont, target, (Vector2){
				screenWidth / 2 - targetSize.x / 2, 140
				}, 70, 1, DARKBLUE);

		// Draw input (centered, bold color)
		Vector2 inputSize = MeasureTextEx(customFont, input, 60, 1);
		DrawTextEx(customFont, input, (Vector2){
				screenWidth / 2 - inputSize.x / 2, 260
				}, 60, 1, MAROON);

		// Draw toast message with slide + fade
		if (showToast) {
			int fontSize = 36;
			const char* message = "✅ Correct!";
			Vector2 msgSize = MeasureTextEx(customFont, message, fontSize, 1);
			int x = screenWidth - msgSize.x - 40;

			float alpha = Clamp((toastY + 50) / 80, 0, 1); // 0 when above, 1 fully visible
			Color bgColor = Fade(GREEN, 0.25f * alpha);
			Color textColor = Fade(GREEN, alpha);

			DrawRectangle(x - 15, (int)toastY - 15, msgSize.x + 30, fontSize + 30, bgColor);
			DrawTextEx(customFont, message, (Vector2){x, (int)toastY}, fontSize, 1.0f, textColor);
		}

		// Draw elapsed time with default font (or change to custom if preferred)
		char timeText[64];
		sprintf(timeText, "Time since start: %.1f seconds", GetTime());
		DrawTextEx(customFont, timeText, (Vector2){10, 10}, 20, 1, BLACK);

		EndDrawing();

	}

	UnloadSound(correctSound);
	UnloadFont(customFont);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

