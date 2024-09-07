#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#define ERROR_IMPL
#include "error.h"
#define CURSOR_IMPL
#include "cursor.h"

#include "config.h"
#include "mat.h"

#define lambda(return_type, function_body) \
    ({ return_type __fn__ function_body __fn__; })

typedef struct {
    int width;
    int height;
    char **data;
} Screen;

void revertTermSettings();

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
    return !(bool)strchr(inActiveChars, c);
}

bool shouldContinueStride()
{
    return rand() % avgStrideLen != 0;
}

bool isExitKey(int key)
{
    return (bool)strchr(exitKeys, key);
}

bool shouldExit()
{
    return isExitKey(readKey());
}

int sleepDeci(int deciSeconds)
{
    return usleep(deciSeconds * 10000);
}

void printColoredChar(char c, const char *color)
{
    printf("%s%c%s", color, c, C0);
}

Screen newScreen()
{
    ERROR e;
    Screen screen;

    if ((e = getScreenSize(&screen.width, &screen.height)) != OK)
        pdie("failed to get screen size", e);

    screen.width = (screen.width / 2);
    screen.height += 1;

    screen.data = (char**)matAlloc(screen.width, screen.height, sizeof(char));
    for (int y = 0; y < screen.height; y++)
        for (int x = 0; x < screen.width; x++)
            screen.data[y][x] = getInactiveChar();

    return screen;
}

void freeScreen(Screen screen)
{
    matFree((void**)screen.data, screen.height);
}

void displayScreen(const Screen screen)
{
    for (int y = 0; y < screen.height; y++) {
        setCursorPos(0, y);
        for (int x = 0; x < screen.width; x++) {
            printColoredChar(screen.data[y][x], activeColor);
            printf(" ");
        }
    }
    updateScreen(); 
}

void moveScreenDown(Screen *screen)
{
    for (int i = screen->height - 1; i > 0; i--) {
        for (int j = 0; j < screen->width; j++) {
            char *c = &screen->data[i][j];
            const bool curr = isActiveChar(screen->data[i][j]);
            const bool next = isActiveChar(screen->data[i - 1][j]);

            if (!curr && next) *c = getActiveChar();
            else if (!next) *c = getInactiveChar();
        }
    }
}

void makeFollowingRow(Screen *screen)
{
    for (int i = 0; i < screen->width; i++) {
        const bool a = shouldContinueStride();
        const bool b = isActiveChar(screen->data[1][i]);
        screen->data[0][i] = (a ^ b) ? getInactiveChar() : getActiveChar();
    }
}

void updateScreenSize(Screen *screen)
{
    freeScreen(*screen);
    *screen = newScreen();
    clearScreen();
}

void revertTermSettings()
{
    showCursor();
    disableRawMode();
    exitAlternativeScreen();
}

void initTermSettings()
{
    srand(time(NULL));
    hideCursor();
    enterAlternativeScreen();
    enableRawMode(0, 0);
    atexit(revertTermSettings);
}

int main(void)
{
    initTermSettings();
    Screen screen = newScreen();

    registerChangeInWindowSize(lambda(void, () {
        updateScreenSize(&screen);
    }));

    while (!shouldExit()) {
        moveScreenDown(&screen);
        makeFollowingRow(&screen);
        displayScreen(screen);
        sleepDeci(delayDeciSeconds);
    }

    freeScreen(screen);
    return EXIT_SUCCESS;
}
