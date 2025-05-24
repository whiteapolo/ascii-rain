#include <bits/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#define LIBZATAR_IMPLEMENTATION
#include "libzatar.h"

#include "config.h"

#define NSEC_IN_SEC 1000000000L

Z_MAT_DECLARE(Screen, char)

static Screen screen = {0};

void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

char getInactiveChar()
{
    return inActiveChars[rand() % inActiveCharsLen];
}

char getActiveChar()
{
    return activeChars[rand() % activeCharsLen];
}

bool isActiveChar(char c)
{
    return !strchr(inActiveChars, c);
}

bool shouldContinueStride()
{
    return rand() % avgStrideLen != 0;
}

bool isExitKey(int key)
{
    return strchr(exitKeys, key);
}

bool shouldExit()
{
    return isExitKey(z_read_key());
}

int sleepDeci(int deciSeconds)
{
    return usleep(deciSeconds * 10000);
}

void initScreen()
{
    int width;
    int height;

    if ((z_get_screen_size(&width, &height)) == Z_Err) {
        die("failed to get screen size");
    }

    if (screen.ptr == NULL) {
        Z_MAT_INIT(&screen, width / 2, height + 1);
    } else {
        Z_MAT_RESIZE(&screen, width / 2, height + 1);
    }


    for (int y = 0; y < screen.y; y++) {
        for (int x = 0; x < screen.x; x++) {
            Z_MAT_AT(&screen, y, x) = getInactiveChar();
        }
    }
}


static Z_Str buf;
void displayScreen()
{
#if 1
    z_set_cursor_pos(0, 0);

    for (int i = 0; i < screen.y; i++) {
        printf("\r\n");
        for (int j = 0; j < screen.x; j++) {
            printf("%c ", Z_MAT_AT(&screen, i, j));
        }
    }

    z_update_screen();
#else
    z_str_clear(&buf);
    z_str_push(&buf, "\033[H");

    for (int i = 0; i < screen.y; i++) {
        z_str_push(&buf, "\r\n");
        for (int j = 0; j < screen.x; j++) {
            z_str_push_c(&buf, Z_MAT_AT(&screen, i, j));
            z_str_push_c(&buf, ' ');
            // z_str_push(&buf, "%c ", Z_MAT_AT(&screen, i, j));
        }
    }

    z_str_print(buf);
    // z_update_screen();
#endif
}

void shiftScreenDown()
{
    for (int i = screen.y - 1; i > 0; i--) {
        for (int j = 0; j < screen.x; j++) {
            const bool isCurrActive = isActiveChar(Z_MAT_AT(&screen, i, j));
            const bool isAboveActive = isActiveChar(Z_MAT_AT(&screen, i - 1, j));

            if (!isCurrActive && isAboveActive) {
                Z_MAT_AT(&screen, i, j) = getActiveChar();
            } else if (!isAboveActive) {
                Z_MAT_AT(&screen, i, j) = getInactiveChar();
            }
        }
    }
}

void generateTopRow()
{
    for (int i = 0; i < screen.x; i++) {
        const bool continueStride = shouldContinueStride();
        const bool isActiveBelow = isActiveChar(Z_MAT_AT(&screen, 1, i));
        Z_MAT_AT(&screen, 0, i) = (continueStride ^ isActiveBelow) ? getInactiveChar() : getActiveChar();
    }
}

void updateScreenSize()
{
    initScreen();
    z_clear_screen();
}

void resetTermSettings()
{
    z_show_cursor();
    z_disable_raw_mode();
    z_exit_alternative_screen();
    printf(Z_COLOR_RESET);
}

void initTermSettings()
{
    srand(time(NULL));
    z_hide_cursor();
    z_enter_alternative_screen();
    z_enable_raw_mode(0, 0);
    atexit(resetTermSettings);
    printf(ACTIVE_COLOR);
}

int main(void)
{
    initTermSettings();
    initScreen();
    z_str_init(&buf, "");

    z_register_change_in_window_size(updateScreenSize);

    struct timespec start;
    struct timespec end;
    long target_ns = delayDeciSeconds * 10000000L;

    while (!shouldExit()) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        shiftScreenDown();
        generateTopRow();
        displayScreen();
        clock_gettime(CLOCK_MONOTONIC, &end);

        long elapsed_ns = (end.tv_sec - start.tv_sec) * NSEC_IN_SEC
            + (end.tv_nsec - start.tv_nsec);

        long remaining_ns = target_ns - elapsed_ns;
        if (remaining_ns > 0) {
            struct timespec sleep_time = {
                .tv_sec = remaining_ns / NSEC_IN_SEC,
                .tv_nsec = remaining_ns % NSEC_IN_SEC
            };
            nanosleep(&sleep_time, NULL);
        }
    }

    Z_MAT_FREE(&screen);

    return EXIT_SUCCESS;
}
