#include <errno.h>
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

#define FOR_RANGE(var, from, to, jumps) for (int var = (from); var < (to); var+=jumps)
#define FOR(var, till) FOR_RANGE(var, 0, till, 1)

typedef struct {
    char c;
    bool active;
} Glypn;

typedef struct {
    int width;
    int height;
    Glypn **data;
} Screen;

const char charcters[] = {
    "!\"#$%&'()*+,-./0123456789:;<=>?@"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
    "abcdefghijklmnopqrstuvwxyz{|}~"
};

static const int charctersLen = (sizeof(charcters) / sizeof(charcters[0])) - 1;
static Screen screen;

char getRandomCharecter();
void init();
void destroy();
void pdie(const char *label, ERROR e);
void die(const char *fmt, ...);
static void *exitMalloc(size_t size);
void **alocateMatrix(int x, int y, int elementSize);
void matSet(void **mat, int x, int y, int elementSize, char value);
void freeMat(void **mat, int y);
void newScreen();
void freeScreen();
void windowSizeChanged();
void printGlypn(Glypn gl);
void refreshScreen(const Screen screen);
void moveScreenDown(Screen *screen);
bool shouldContinueStride();
void makeFollowingRow(Screen *screen);

char getRandomCharecter()
{
    return charcters[rand() % charctersLen];
}

void init()
{
    srand(time(NULL));
    hideCursor();
    enterAlternativeScreen();
    disableLineWrap();
    enableRawMode(0, 0);
    enableFullBuffering(stdout);
    registerChangeInWindowSize(windowSizeChanged);
    newScreen();
}

void destroy()
{
    freeScreen();
    showCursor();
    disableRawMode();
    enableLineWrap();
    exitAlternativeScreen();
}

void pdie(const char *label, ERROR e)
{
	printError(label, e);
    destroy();
	exit(EXIT_FAILURE);
}

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    destroy();
    exit(EXIT_FAILURE);
}

static void *exitMalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL)
        die("exitMalloc");
	return ptr;
}

void **alocateMatrix(int x, int y, int elementSize)
{
    void **mat = exitMalloc(sizeof(void*) * y);
    FOR (i, y)
        mat[i] = exitMalloc(elementSize * x);
    return mat;
}

void matSet(void **mat, int x, int y, int elementSize, char value)
{
    FOR (i, y)
        memset(mat[i], x * elementSize, value);
}

void freeMat(void **mat, int y)
{
    FOR (i, y)
        free(mat[i]);
    free(mat);
}

void newScreen()
{
    ERROR e;

    if ((e = getScreenSize(&screen.width, &screen.height)) != OK)
        pdie("newScreen", e);

    screen.width /= 2;
    screen.height += 1;

    screen.data = (Glypn**)alocateMatrix(screen.width, screen.height, sizeof(Glypn));
    matSet((void**)screen.data, screen.width, screen.height, sizeof(Glypn), 0);
}

void freeScreen()
{
    freeMat((void**)screen.data, screen.height);
}

void windowSizeChanged()
{
    freeScreen();
    newScreen();
}

void printGlypn(Glypn gl)
{
    printf(C2 "%c" C0, gl.active ? gl.c : ' ');
}

void refreshScreen(const Screen screen)
{
    setCursorPos(0, 0);
    FOR(i, screen.height) {
        setCursorPos(0, i);
        FOR(j, screen.width) {
            printGlypn(screen.data[i][j]);
            printf(" ");
        }
    }
}

void moveScreenDown(Screen *screen)
{
    for (int i = screen->height - 1; i > 0; i--) {
        for (int j = 0; j < screen->width; j++) {
            Glypn *gl = &screen->data[i][j];
            const bool before = screen->data[i][j].active;
            const bool after = screen->data[i - 1][j].active;
            gl->active = after;
            if (!before && after)
                gl->c = getRandomCharecter();
        }
    }
}

bool shouldContinueStride()
{
    return rand() % 8 != 0;
}

void makeFollowingRow(Screen *screen)
{
    FOR (i, screen->width) {
        if (shouldContinueStride()) {
            screen->data[0][i].c = getRandomCharecter();
            screen->data[0][i].active = screen->data[1][i].active;
        } else {
            screen->data[0][i].active = !screen->data[1][i].active;
        }
    }
}

int main(void)
{
    init();

    int key = readKey();

    while (key != 'q') {
        moveScreenDown(&screen);
        makeFollowingRow(&screen);
        refreshScreen(screen);
        key = readKey();
        usleep(40000);
    }

    destroy();
    return EXIT_SUCCESS;
}
