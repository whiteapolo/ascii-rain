#ifndef MAT_H
#define MAT_H

void *exitMalloc(int size);
void **matAlloc(int x, int y, int elementSize);
void matFree(void **mat, int y);

#endif
