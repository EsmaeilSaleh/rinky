#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_LEN 100

const char* words[] = {
    "loo", "llo", "lol", "pol", "pl", "9l9", "l;", ";l"
};
int num_words = sizeof(words) / sizeof(words[0]);

// Set terminal to raw mode (no Enter, no echo)
void set_terminal_mode(struct termios* oldt) {
    struct termios newt;
    tcgetattr(STDIN_FILENO, oldt);
    newt = *oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Restore terminal to previous mode
void reset_terminal_mode(struct termios* oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

// Time in milliseconds
long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

int main() {
    srand(time(NULL));
    int score = 0, attempts = 0;

    printf("⌨️  Word Trainer (press 'q' anytime to quit)\n");

    struct termios oldt;
    set_terminal_mode(&oldt);  // enter raw mode

    while (1) {
        const char* target = words[rand() % num_words];
        int len = strlen(target);
        char input[MAX_LEN] = {0};
        int index = 0;

        printf("👉 Type this: %s\n", target);
        fflush(stdout);

        long long start = current_time_ms();

        while (index < len) {
            char ch = getchar();

            if (ch == 'q') {
                reset_terminal_mode(&oldt);
                printf("\n👋 Goodbye!\n");
                return 0;
            }

            input[index++] = ch;
            write(STDOUT_FILENO, &ch, 1); // show character
        }

        long long elapsed = current_time_ms() - start;
        input[index] = '\0'; // null-terminate

        attempts++;

        if (strcmp(input, target) == 0) {
            printf(" ✔️  (%lld ms)\n\n", elapsed);
            score++;
        } else {
            printf(" ❌  (you typed '%s') — %lld ms\n\n", input, elapsed);
        }
    }

    reset_terminal_mode(&oldt);
    printf("\n🏁 Final score: %d/%d\n", score, attempts);
    return 0;
}
