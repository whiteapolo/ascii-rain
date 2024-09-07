#include "mat.h"
#include <stdlib.h>

// char matrix

void die(const char *fmt, ...);

void *exitMalloc(int size)
{
	void *ptr = malloc(size);
	if (ptr == NULL)
        die("failed to malloc");
	return ptr;
}

void **matAlloc(int x, int y, int elementSize)
{
    void **mat = exitMalloc(sizeof(void*) * y);
    for (int i = 0; i < y; i++)
        mat[i] = exitMalloc(elementSize * x);
    return mat;
}

void matFree(void **mat, int y)
{
    for (int i = 0; i < y; i++)
        free(mat[i]);
    free(mat);
}
