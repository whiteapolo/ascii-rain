/*
           Libzatar

	Libraries in this file:
		- path
		- cursor

	Data Structures:
		- Vector
		- Stack
		- Heap
		- PriorityQueue
		- List
		- CircularList
		- Queue
		- AvlTree
		- Map
*/

// TODO: circular list to camel case

#ifndef LIBZATAR_H
#define LIBZATAR_H

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))
#define IN_RANGE(min, x, max) ((min) <= (x) && (x) <= (max))

typedef enum {
	Ok = 0,
	Err = -1,
} Result;

/*********************************************


                Vector - HEADER


**********************************************/
#define MIN_VECTOR_CAPACITY 0

typedef struct {
	void **data;
	u64 size;
	u64 capacity;
} Vector;

Vector *newVecWithCapacity(u64 initialCapacity);
Vector *newVec();

void *vecAt(const Vector *v, u64 i);
u64 vecSize(const Vector *v);
bool vecIsEmpty(const Vector *v);

void vecAdd(Vector *v, void *data);
void *vecRemoveLast(Vector *v);
void vecInsertAt(Vector *v, u64 i, void *data);
void *vecRemoveAt(Vector *v, u64 i);

void *vecReplace(Vector *v, u64 i, void *data);
void vecSwap(Vector *v, u64 i, u64 j);
void vecRemoveRange(Vector *v, i64 start, i64 end, i64 step, void freeData(void *));
void vecAddVec(Vector *dst, Vector *src);
void vecAddVecAt(Vector *dest, u64 i, Vector *src);

Vector *vecClone(const Vector *v, void *dupData(const void *));
Vector *vecCloneRange(const Vector *v, i64 start, i64 end, i64 step, void *dupData(const void *));
void vecShrinkToFit(Vector *v);

void vecClear(Vector *v, void freeData(void *));
void vecFree(Vector *v, void freeData(void *));

void vecSort(Vector *v, int cmp(const void *, const void *));
void vecReverse(Vector *v);
void vecShuffle(Vector *v);
void vecReduce(Vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *));
void vecMap(Vector *v, void *transform(const void *));
void vecMapRange(Vector *v, i64 start, i64 end, i64 step, void *transform(const void *));
void vecForEach(const Vector *v, void action(const void *));
void vecForEachRange(const Vector *v, i64 start, i64 end, i64 step, void action(const void *));
void vecPrint(const Vector *v, void printData(const void *));

/*********************************************


                Stack - HEADER


**********************************************/

typedef Vector Stack;

#define MIN_STACK_CAPACITY 0

Stack *newStack();
Stack *newStackWithCapacity(const u64 capacity);
void stackPush(Stack *s, void *data);
void *stackPop(Stack *s);
const void *stackTop(const Stack *s);
u64 stackSize(const Stack *s);
bool stackIsEmpty(const Stack *s);
void stackShrinkToFit(Stack *s);
void stackClear(Stack *s, void freeData(void *));
void stackFree(Stack *s, void freeData(void *));
void stackPrint(const Vector *v, void printData(const void *));

/*********************************************


                Heap - HEADER


**********************************************/

#define MIN_HEAP_CAPACITY 0

typedef struct {
	void **vec;
	u64 size;
	u64 capacity;
	int (*cmp)(const void *, const void *);
} Heap;

Heap *newHeap(int cmp(const void *, const void *));
Heap *newHeapWithCapacity(int cmp(const void *, const void *), const u64 initialCapacity);
void heapPush(Heap *h, void *data);
const void *heapPeek(const Heap *h);
void *heapPop(Heap *h);
u64 heapSize(const Heap *h);
bool heapIsEmpty(const Heap *h);
void heapShrinkToFit(Heap *h);
void heapClear(Heap *h, void freeData(void *));
void heapFree(Heap *h, void freeData(void *));

/*********************************************


            Priority Queue - HEADER


**********************************************/

#define MIN_PRIORITY_QUEUE_CAPACITY 16

typedef Heap PriorityQueue;

PriorityQueue *newPriorityQueue(int cmp(const void *, const void *));
PriorityQueue *newPriorityQueueWithCapacity(int cmp(const void *, const void *), const u64 initialCapacity);
void priorityQueuePush(PriorityQueue *q, void *data);
void *priorityQueuePop(PriorityQueue *q);
const void *priorityQueuePeek(const PriorityQueue *q);
u64 priorityQueueGetSize(const PriorityQueue *q);
bool priorityQueueIsEmpty(const PriorityQueue *q);
void priorityQueueShrinkToFit(PriorityQueue *q);
void priorityQueueClear(PriorityQueue *q, void freeData(void *));
void priorityQueueFree(PriorityQueue *q, void freeData(void *));


/*********************************************


                List - HEADER


**********************************************/

typedef struct List{
	struct List *next;
	void *data;
} List;

List *newList();
void listPush(List **m, void *data);
void *listPop(List **m);
void listInsertAfter(List *l, void *data);
void *listRemoveAfter(List *l);
void listFree(List *m, void freeData(void *));
void listInsertEnd(List *l, void *data);
void listPrint(const List *l, void printData(const void*));
const List *listGetLast(const List *l);
void listReverse(List **lst);

/*********************************************


            Circular list - HEADER


**********************************************/

typedef struct circularList {
	struct circularList *next;
	void *data;
} circularList;

circularList *newCircularList();
void circularListPush(circularList **cl, void *data);
void *circularListPop(circularList **cl);
void circularListInsertAfter(circularList *cl, void *data);
void circularListInsertLast(circularList **cl, void *data);
void *circularListRemoveAfter(circularList *cl);
void circularListFree(circularList *cl, void freeData(void *));
void circularListPrint(circularList *cl, void printData(const void *));

/*********************************************


                Queue - HEADER


**********************************************/

typedef struct {
	circularList *cl;
	u64 size;
} Queue;

Queue *newQueue();
void queuePush(Queue *q, void *data);
void *queuePop(Queue *q);
void *queuePeek(Queue *q);
bool queueIsEmpty(const Queue *q);
u64 queueSize(const Queue *q);
void queuePrint(const Queue *q, void printData(const void *));
void queueClear(Queue *q, void freeData(void *));
void queueFree(Queue *q, void freeData(void *));
circularList *queueToCircularList(Queue *q);

/*********************************************


                AvlTree - HEADER


**********************************************/

typedef struct avlNode {
	struct avlNode *left;
	struct avlNode *right;
	void *key;
	void *data;
	int height;
} avlNode;

avlNode *newAvlNode(void *key, void *data);
avlNode *avlGetMin(avlNode *root);
avlNode *avlGetMax(avlNode *root);
const avlNode *avlFindNode(const avlNode *root, const void *key, int cmpKeys(const void *,const void *));
bool avlIsExists(const avlNode *root, const void *key, int cmpKeys(const void *,const void *));
void *avlFind(const avlNode *root, const void *key, int cmpKeys(const void *,const void *));
void avlInsert(avlNode **root, void *key, void *data, int cmpKeys(const void *,const void *));
void avlRemove(avlNode **root, const void *key, int cmpKeys(const void *,const void *), void freeKey(void *), void freeData(void *));
void avlUpdate(avlNode *root, const void *key, int cmpKeys(const void *,const void *), void freeData(void *), void *newData);
void avlOrderTraverse(const avlNode *root, void action(const void *key, const void *data, void *arg), void *arg);
void avlPrint(const avlNode *root, void print(const void *key, const void *data, void *arg), void *arg, int padding);
void avlFree(avlNode *root, void freeKey(void *), void freeData(void *));

/*********************************************


                Map - HEADER


**********************************************/

typedef struct {
	avlNode *root;
	int (*cmpKeys)(const void *, const void *);
} Map;

Map *newMap(int cmpKeys(const void *, const void *));
void mapInsert(Map *m, void *key, void *data);
void *mapFind(const Map *m, const void *key);
bool mapIsExists(const Map *m, const void *key);
void mapRemove(Map *m, const void *key, void freeKey(void *), void freeData(void *));
void mapUpdate(Map *m, const void *key, void freeData(void *), void *newData);
void mapOrderTraverse(const Map *m, void action(const void *key, const void *data, void *arg), void *arg);
void mapFree(Map *m, void freeKey(void *), void freeData(void *));
bool mapIsEmpty(const Map *m);


/*********************************************


                String - HEADER


**********************************************/

u64 getFileSize(FILE *fp);
i32 getFmtSize(const char *fmt, ...);
i32 getFmtSizeVa(const char *fmt, va_list ap);
void *memdup(const void *mem, const size_t size);
void swap(void *a, void *b, const size_t size);
int zatarMax(int a, int b);
int zatarMin(int a, int b);
int zatarMin3(int a, int b, int c);
int zatarMax3(int a, int b, int c);

/*********************************************


                Cursor - HEADER


**********************************************/
#define CTRL_KEY(k) ((k) & 0x1f)

#define C0  "\033[0m"     /*  RESET        */

#define C1  "\033[0;31m"  /*  RED          */
#define C2  "\033[0;32m"  /*  GREEN        */
#define C3  "\033[0;33m"  /*  YELLOW       */
#define C4  "\033[0;34m"  /*  BLUE         */
#define C5  "\033[0;035m" /*  MAGENTA      */
#define C6  "\033[0;36m"  /*  CYAN         */
#define C7  "\033[0;37m"  /*  WHITE        */
#define C8  "\033[0;90m"  /*  GRAY         */

#define B1  "\033[1;91m"  /*  BOLD RED     */
#define B2  "\033[1;92m"  /*  BOLD GREEN   */
#define B3  "\033[1;93m"  /*  BOLD YELLOW  */
#define B4  "\033[1;94m"  /*  BOLD BLUE    */
#define B5  "\033[1;95m"  /*  BOLD MAGENTA */
#define B6  "\033[1;96m"  /*  BOLD CYAN    */
#define B7  "\033[1;97m"  /*  BOLD WHITE   */
#define B8  "\033[1;90m"  /*  BOLD GRAY    */

enum {
    EMPTY_KEY = 999,

	ARROW_LEFT = 1000,
	ARROW_RIGHT = 1001,
	ARROW_UP = 1002,
	ARROW_DOWN = 1003,

	PAGE_UP = 1007,
	PAGE_DOWN = 1008,

	DELETE = 1004,
	HOME = 1005,
	END = 1006,
};

typedef enum {
	BLOCK_STEADY = 0,
	BLOCK_BLINKING = 1,
	UNDERLINE_BLINKING = 3,
	UNDERLINE_STEADY = 4,
	BEAM_STEADY = 6,
	BEAM_BLINKING = 5,
} CURSOR_STYLE;

Result enableRawMode(int vminKeys, int vtime);
Result disableRawMode();

void disableLineWrap();
void enableLineWrap();

void hideCursor();
void showCursor();

void setCursorStyle(CURSOR_STYLE style);

Result getCursorPos(int *x, int *y);
void setCursorPos(int x, int y);

void setCursorX(int x);

void cursorUp(int n);
void cursorDown(int n);
void cursorRight(int n);
void cursorLeft(int n);

// unsupported on some terminals
void saveCursorPos();
void restoreCursorPos();

void enterAlternativeScreen();
void exitAlternativeScreen();

void clearLine();
void clearScreen();

void updateScreen();

Result getScreenSizeByCursor(int *width, int *height);
Result getScreenSizeByIoctl(int *width, int *height);
Result getScreenSize(int *width, int *height);

Result registerChangeInWindowSize(void funciton(int));

Result enableFullBuffering(FILE *fp);

int waitForByte();
int readEscapeKey();
int readKey();


/*********************************************


                 Path - HEADER


**********************************************/

typedef enum {
	Read = 0,
	Write = 1,
} PipeMode;

const char *getPathExtention(const char *path);
const char *getHomePath();
char *expandPath(const char *path);
void compressPath(char *path);
bool isExtentionEqual(const char *path, const char *extention);

int dirTraverse(const char *dir, bool action(const char *));
int traverseFile(const char *fileName, int bufSize, bool action(char[bufSize]));

bool isDir(const char *path);
bool isRegularFile(const char *fileName);
bool isPathExists(const char *path);

int echoFileWrite(const char *fileName, const char *fmt, ...);
int echoFileAppend(const char *fileName, const char *fmt, ...);
int readFile(const char *fileName, const char *fmt, ...);
int redirectFd(int srcFd, const char *destFileName);
int popen2(char *path, char *argv[], FILE *ppipe[2]);

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen);

int nextInDir(DIR *dir, const char *dirName, char *destFileName, int destLen);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
size_t getFileSize(FILE *fp);

/*============================================


                -End-Headers-


=============================================*/
#ifdef LIBZATAR_IMPL

/*********************************************


             Vector IMPLEMENTATION


**********************************************/
typedef struct {
	i64 start;
	i64 step;
	i64 end;
	i64 i;
} VecIter;

VecIter newVecIter(const Vector *v, i64 start, i64 end, i64 step)
{
	assert(step != 0);

	VecIter vi = {
		.start = start,
		.end = end,
		.step = step,
	};

	// wrap negative values to the end of the array
	if (vi.start < 0)
		vi.start += vecSize(v);
	if (vi.end < 0)
		vi.end += vecSize(v);
	return vi;
}

i64 VecIterBegin(VecIter *vi)
{
	vi->i = vi->start;
	return vi->i;
}

i64 VecIterNext(VecIter *vi)
{
	vi->i += vi->step;
	return vi->i;
}

bool VecIterIsEnd(const VecIter *vi)
{
	if (vi->step > 0 && vi->i > vi->end)
		return true;
	if (vi->step < 0 && vi->i < vi->end)
		return true;
	return false;
}

Vector *newVecWithCapacity(u64 initialCapacity)
{
	Vector *v = malloc(sizeof(Vector));
	v->size = 0;
	v->capacity = initialCapacity;
	v->data = malloc(sizeof(void *) * initialCapacity);

	return v;
}

Vector *newVec()
{
	return newVecWithCapacity(MIN_VECTOR_CAPACITY);
}

void *vecAt(const Vector *v, u64 i)
{
	assert(i < v->size);

	return v->data[i];
}

void vecAdd(Vector *v, void *data)
{
	v->size++;

	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}

	v->data[v->size - 1] = data;
}

void *vecRemoveLast(Vector *v)
{
	return v->data[--v->size];
}

void vecInsertAt(Vector *v, u64 i, void *data)
{
	assert(i <= v->size);
	v->size++;

	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(void *) * v->capacity);
	}

	memmove(&v->data[i+1], &v->data[i], sizeof(void *) * (v->size - i - 1));
	v->data[i] = data;
}

void *vecRemoveAt(Vector *v, u64 i)
{
	assert(i < v->size);
	void *dataToRemove = v->data[i];
	memmove(&v->data[i], &v->data[i+1], sizeof(void *) * (v->size - i - 1));
	v->size--;

	return dataToRemove;
}

void *vecReplace(Vector *v, u64 i, void *data)
{
	void *tmp = v->data[i];
	v->data[i] = data;
	return tmp;
}

void vecSwap(Vector *v, u64 i, u64 j)
{
	swap(&v->data[i], &v->data[j], sizeof(void *));
}

void vecRemoveRange(Vector *v, i64 start, i64 end, i64 step, void freeData(void *))
{
	u8 tmpPtr;
	VecIter vi = newVecIter(v, start, end, step);

	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi)) {
		if (freeData)
			freeData(vecReplace(v, i, &tmpPtr));
		else
			vecReplace(v, i, &tmpPtr);
	}

	Vector *tmp = newVecWithCapacity(vecSize(v));
	for (u64 i = 0; i < vecSize(v); i++)
		vecAdd(tmp, (void *)vecAt(v, i));
	vecFree(v, NULL);
	*v = *tmp;
}

void vecAddVec(Vector *dst, Vector *src)
{
	for (u64 i = 0; i < vecSize(src); i++)
		vecAdd(dst, (void *)vecAt(src, i));
	vecFree(src, NULL);
}

void vecAddVecAt(Vector *dest, u64 i, Vector *src)
{
	u64 oldDestSize = dest->size;
	dest->size += src->size;

	if (dest->size > dest->capacity) {
		dest->capacity = dest->size * 2;
		dest->data = realloc(dest->data, sizeof(void *) * dest->capacity);
	}

	memmove(&dest->data[i + src->size], &dest->data[i], sizeof(void *) * (oldDestSize - i));
	memcpy(&dest->data[i], &src->data[0], src->size);
	vecFree(src, NULL);
}

u64 vecSize(const Vector *v)
{
	return v->size;
}

bool vecIsEmpty(const Vector *v)
{
	return vecSize(v) == 0;
}

Vector *vecClone(const Vector *v, void *dupData(const void *))
{
	Vector *new = newVecWithCapacity(vecSize(v));
	for (u64 i = 0; i < vecSize(v); i++)
		vecAdd(new, dupData(vecAt(v, i)));
	return new;
}

Vector *vecCloneRange(const Vector *v, i64 start, i64 end, i64 step, void *dupData(const void *))
{
	Vector *clone = newVecWithCapacity(vecSize(v));
	VecIter vi = newVecIter(v, start, end, step);

	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		vecAdd(clone, dupData(vecAt(v, i)));

	return clone;
}

void vecShrinkToFit(Vector *v)
{
	v->capacity = v->size;
	v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void vecClear(Vector *v, void freeData(void *))
{
	if (freeData)
		for (u64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));
	v->size = 0;
}

void vecFree(Vector *v, void freeData(void *))
{
	if (freeData)
		for (u64 i = 0; i < vecSize(v); i++)
			freeData((void *)vecAt(v, i));

	free(v->data);
	free(v);
}

void vecSort(Vector *v, int cmp(const void *, const void *))
{
	qsort(v->data, v->size, sizeof(void *), cmp);
}

void vecReverse(Vector *v)
{
	u64 start = 0;
	u64 end = vecSize(v) - 1;

	while (start < end)
		vecSwap(v, start++, end--);
}

void vecShuffle(Vector *v)
{
	for (u64 i = 0; i < vecSize(v); i++) {
		i64 j = rand() % vecSize(v);
		vecSwap(v, i, j);
	}
}

void vecReduce(Vector *v, bool shouldRemove(const void *, i64 i), void freeData(void *))
{
	Vector *tmp = newVecWithCapacity(v->capacity);
	for (u64 i = 0; i < vecSize(v); i++) {
		void *currData = (void *)vecAt(v, i);
		if (!shouldRemove(currData, i))
			vecAdd(tmp, currData);
		else if (freeData)
			freeData(currData);
	}
	vecFree(v, NULL);
	*v = *tmp;
}

void vecMap(Vector *v, void *transform(const void *))
{
	for (u64 i = 0; i < vecSize(v); i++)
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecMapRange(Vector *v, i64 start, i64 end, i64 step, void *transform(const void *))
{
	VecIter vi = newVecIter(v, start, end, step);
	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		vecReplace(v, i, transform(vecAt(v, i)));
}

void vecForEach(const Vector *v, void action(const void *))
{
	for (u64 i = 0; i < vecSize(v); i++)
		action(vecAt(v, i));
}

void vecForEachRange(const Vector *v, i64 start, i64 end, i64 step, void action(const void *))
{
	VecIter vi = newVecIter(v, start, end, step);
	for (i64 i = VecIterBegin(&vi); VecIterIsEnd(&vi); i = VecIterNext(&vi))
		action(vecAt(v, i));
}

void vecPrint(const Vector *v, void printData(const void *))
{
	if (v->size == 0) {
		printf("[]\n");
		return;
	}

	printf("[ ");

	for (u64 i = 0; i < v->size - 1; i++) {
		printData(v->data[i]);
		printf(", ");
	}

	printData(v->data[v->size - 1]);


	printf(" ]\n");
}

/*********************************************


              Stack IMPLEMENTATION


**********************************************/

Stack *newStack()
{
	return newStackWithCapacity(MIN_STACK_CAPACITY);
}

Stack *newStackWithCapacity(u64 capacity)
{
	return newVecWithCapacity(capacity);
}

void stackPush(Stack *s, void *data)
{
	vecAdd(s, data);
}

void *stackPop(Stack *s)
{
	return vecRemoveLast(s);
}

const void *stackTop(const Stack *s)
{
	return vecAt(s, vecSize(s) - 1);
}

u64 stackSize(const Stack *s)
{
	return vecSize(s);
}

bool stackIsEmpty(const Stack *s)
{
	return vecIsEmpty(s);
}

void stackShrinkToFit(Stack *s)
{
	vecShrinkToFit(s);
}

void stackClear(Stack *s, void freeData(void *))
{
	vecClear(s, freeData);
}

void stackFree(Stack *s, void freeData(void *))
{
	vecFree(s, freeData);
}

void stackPrint(const Stack *s, void printData(const void *))
{
	vecPrint(s, printData);
}

/*********************************************


			 Heap IMPLEMENTATION


**********************************************/

u64 parent(u64 i);
u64 right(u64 i);
u64 left(u64 i);
void heapifyUp(Heap *h, u64 i);
void heapifyDown(Heap *h, u64 i);

Heap *newHeapWithCapacity(int cmp(const void *, const void *), const u64 initialCapacity)
{
	Heap *h = malloc(sizeof(Heap));
	h->size = 0;
	h->capacity = initialCapacity;
	h->vec = malloc(sizeof(void*) * h->capacity);
	h->cmp = cmp;

	return h;
}

Heap *newHeap(int cmp(const void *, const void *))
{
	return newHeapWithCapacity(cmp, MIN_HEAP_CAPACITY);
}

u64 parent(u64 i)
{
	return i / 2;
}

u64 right(u64 i)
{
	return (i * 2) + 2;
}

u64 left(u64 i)
{
	return (i * 2) + 1;
}

void heapifyUp(Heap *h, u64 i)
{
	if (i == 0) {
		return;
	}

	if (h->cmp(h->vec[parent(i)], h->vec[i]) > 0) {
		swap(&h->vec[i], &h->vec[parent(i)], sizeof(void*));
		heapifyUp(h, parent(i));
	}
}

void heapifyDown(Heap *h, u64 i)
{
	u64 l = left(i);
	u64 r = right(i);

	u64 largest = i;

	if (l < h->size && h->cmp(h->vec[i], h->vec[l]) > 0) {
		largest = l;
	}

	if (r < h->size && h->cmp(h->vec[largest], h->vec[r]) > 0) {
		largest = r;
	}

	if (largest != i) {
		swap(&h->vec[i], &h->vec[largest], sizeof(void*));
		heapifyDown(h, largest);
	}
}

void heapPush(Heap *h, void *data)
{
	h->size++;

	if (h->size > h->capacity) {
		h->capacity *= 2;
		h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
	}

	h->vec[h->size - 1] = data;
	heapifyUp(h, h->size - 1);
}

const void *heapPeek(const Heap *h)
{
	return h->vec[0];
}

void *heapPop(Heap *h)
{
	void *ret = h->vec[0];
	h->vec[0] = h->vec[--h->size];
	heapifyDown(h, 0);

	return ret;
}

u64 heapSize(const Heap *h)
{
	return h->size;
}

bool heapIsEmpty(const Heap *h)
{
	return heapSize(h) == 0;
}

void heapShrinkToFit(Heap *h)
{
	h->capacity = h->size;
	h->vec = realloc(h->vec, sizeof(void*) * h->capacity);
}

void heapClear(Heap *h, void freeData(void *))
{
	if (freeData) {
		for (u64 i = 0; i < h->size; i++) {
			freeData(h->vec[i]);
		}
	}

	h->size = 0;
}

void heapFree(Heap *h, void freeData(void *))
{
	heapClear(h, freeData);
	free(h->vec);
	free(h);
}

/*********************************************


			 Priority Queue IMPLEMENTATION


**********************************************/

PriorityQueue *newPriorityQueueWithCapacity(int cmp(const void *, const void *), const u64 initialCapacity)
{
	return newHeapWithCapacity(cmp, initialCapacity);
}

PriorityQueue *newPriorityQueue(int cmp(const void *, const void *))
{
	return newHeapWithCapacity(cmp, MIN_PRIORITY_QUEUE_CAPACITY);
}

void priorityQueuePush(PriorityQueue *q, void *data)
{
	heapPush(q, data);
}

void *priorityQueuePop(PriorityQueue *q)
{
	return heapPop(q);
}

const void *priorityQueuePeek(const PriorityQueue *q)
{
	return heapPeek(q);
}

u64 priorityQueueGetSize(const PriorityQueue *q)
{
	return heapSize(q);
}

bool priorityQueueIsEmpty(const PriorityQueue *q)
{
	return heapIsEmpty(q);
}

void priorityQueueShrinkToFit(PriorityQueue *q)
{
	heapShrinkToFit(q);
}

void priorityQueueClear(PriorityQueue *q, void freeData(void *))
{
	heapClear(q, freeData);
}

void priorityQueueFree(PriorityQueue *q, void freeData(void *))
{
	heapFree(q, freeData);
}


/*********************************************


			 List IMPLEMENTATION


**********************************************/

List *newList()
{
	return NULL;
}

void listPush(List **m, void *data)
{
	List *n = malloc(sizeof(List));
	n->next = *m;
	n->data = data;
	*m = n;
}

void *listPop(List **m)
{
	void *data = (*m)->data;
	List *tmp = *m;
	*m = (*m)->next;
	free(tmp);
	return data;
}

void listInsertAfter(List *l, void *data)
{
	List *n = malloc(sizeof(List));
	n->next = l->next;
	n->data = data;
	l->next = n;
}

void *listRemoveAfter(List *l)
{
	List *tmp = l->next;
	void *data = tmp->data;
	l->next = l->next->next;
	free(tmp);

	return data;
}

void listFree(List *m, void freeData(void *))
{
	while (m) {
		void *tmp = listPop(&m);
		if (freeData) {
			freeData(tmp);
		}
	}
}

void listInsertEnd(List *l, void *data)
{
	listInsertAfter((List *)listGetLast(l), data);
}

void listPrint(const List *l, void printData(const void *))
{
	while (l) {
		printData(l->data);
		l = l->next;
	}
}

const List *listGetLast(const List *l)
{
	while (l->next) {
		l =  l->next;
	}

	return l;
}

void listReverse(List **lst)
{
	List *right = *lst;
	List *left = *lst;

	while (right->next) {
		void *tmp = listRemoveAfter(right);
		listPush(&left, tmp);
	}

	*lst = left;
}


/*********************************************


         Circular List IMPLEMENTATION


**********************************************/
circularList *newCircularList()
{
	return NULL;
}

void circularListPush(circularList **cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;
	if (*cl == NULL) {
		n->next = n;
		*cl = n;
	} else if ((*cl)->next == (*cl)) {
		n->next = *cl;
		(*cl)->next = n;
	} else {
		n->next = (*cl)->next;
		(*cl)->next = n;
	}
}

void *circularListPop(circularList **cl)
{
	circularList *toRemove = (*cl)->next;
	void *data = toRemove->data;

	if (*cl == toRemove) {

		*cl = NULL;
	} else {
		(*cl)->next = (*cl)->next->next;
	}

	free(toRemove);

	return data;
}

void circularListInsertAfter(circularList *cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;
	n->next = cl->next;
	cl->next = n;
}

void *circularListRemoveAfter(circularList *cl)
{
	circularList *toRemove = cl->next;
	cl->next = cl->next->next;
	void *tmp = toRemove->data;
	free(toRemove);

	return tmp;
}

void circularListInsertLast(circularList **cl, void *data)
{
	circularList *n = malloc(sizeof(circularList));
	n->data = data;

	if (*cl == NULL) {
		n->next = n;
	} else if ((*cl)->next == (*cl)) {
		n->next = *cl;
		(*cl)->next = n;
	} else {
		n->next = (*cl)->next;
		(*cl)->next = n;
	}

	*cl = n;
}

void circularListFree(circularList *cl, void freeData(void *))
{
	if (freeData) {
		while (cl) {
			freeData(circularListPop(&cl));
		}
	} else {
		while (cl) {
			circularListPop(&cl);
		}
	}
}

void circularListPrint(circularList *cl, void printData(const void *))
{
	if (cl == NULL) {
		printf("{}\n");
		return;
	}

	printf("{ ");

	for (circularList *curr = cl->next; curr != cl; curr = curr->next) {
		printData(curr->data);
		printf(", ");
	}

	printData(cl->data);

	printf(" }\n");
}

/*********************************************


			  Queue IMPLEMENTATION


**********************************************/

Queue *newQueue()
{
	Queue *q = malloc(sizeof(Queue));
	q->size = 0;
	q->cl = NULL;

	return q;
}

void queuePush(Queue *q, void *data)
{
	q->size++;
	circularListInsertLast(&q->cl, data);
}

void *queuePop(Queue *q)
{
	q->size--;
	return circularListPop(&q->cl);
}

void *queuePeek(Queue *q)
{
	return q->cl->next->data;
}

bool queueIsEmpty(const Queue *q)
{
	return q->size == 0;
}

u64 queueSize(const Queue *q)
{
	return q->size;
}

void queuePrint(const Queue *q, void printData(const void *))
{
	circularListPrint(q->cl, printData);
}

void queueClear(Queue *q, void freeData(void *))
{
	circularListFree(q->cl, freeData);
	q->size = 0;
	q->cl = NULL;
}

void queueFree(Queue *q, void freeData(void *))
{
	circularListFree(q->cl, freeData);
	free(q);
}

circularList *queueToCircularList(Queue *q)
{
	return q->cl;
}

/*********************************************


             AvlTree IMPLEMENTATION


**********************************************/

avlNode *newAvlNode(void *key, void *data)
{
	avlNode *n = malloc(sizeof(avlNode));
	n->key = key;
	n->data = data;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}

int getHeight(const avlNode *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}

void updateHeight(avlNode *node)
{
	node->height = 1 + zatarMax(getHeight(node->right), getHeight(node->left));
}

int getBalanceFactor(const avlNode *node)
{
	if (node == NULL)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

avlNode *avlGetMin(avlNode *root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

avlNode *avlGetMax(avlNode *root)
{
	while (root->right != NULL)
		root = root->right;
	return root;
}

void leftRotate(avlNode **root)
{
	avlNode *newRoot = (*root)->right;
	avlNode *tmp = newRoot->left;

	newRoot->left = *root;
	(*root)->right = tmp;

	updateHeight(newRoot->left);
	updateHeight(newRoot);

	*root = newRoot;
}

void rightRotate(avlNode **root)
{
	avlNode *newRoot = (*root)->left;
	avlNode *tmp = newRoot->right;

	newRoot->right = *root;
	(*root)->left = tmp;

	updateHeight(newRoot->right);
	updateHeight(newRoot);

	*root = newRoot;
}

void leftRightRotate(avlNode **root)
{
	leftRotate(&(*root)->left);
	rightRotate(root);
}

void rightLeftRotate(avlNode **root)
{
	rightRotate(&(*root)->right);
	leftRotate(root);
}

const avlNode *avlFindNode(const avlNode *root, const void *key, int cmpKeys(const void *,const void *))
{
	int cmpRes;
	while (root && (cmpRes = cmpKeys(key, root->key))) {
		if (cmpRes > 0)
			root = root->right;
		else
			root = root->left;
	}
	return root;
}

bool avlIsExists(const avlNode *root, const void *key, int cmpKeys(const void *,const void *))
{
	return avlFindNode(root, key, cmpKeys) != NULL;
}

void *avlFind(const avlNode *root, const void *key, int cmpKeys(const void *,const void *))
{
	const avlNode *node = avlFindNode(root, key, cmpKeys);
	if (node != NULL)
		return node->data;
	return NULL;
}

void avlInsert(avlNode **root, void *key, void *data, int cmpKeys(const void *,const void *))
{
	if (*root == NULL) {
		*root = newAvlNode(key, data);
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avlInsert(&(*root)->right, key, data, cmpKeys);
	} else {
		avlInsert(&(*root)->left, key, data, cmpKeys);
	}

	updateHeight(*root);
	int bf = getBalanceFactor(*root);

	if (bf > 1 && cmpKeys(key, (*root)->left->key) < 0)
		rightRotate(root);
	else if (bf < -1 && cmpKeys(key, (*root)->right->key) > 0)
		leftRotate(root);
	else if (bf > 1 && cmpKeys(key, (*root)->left->key) > 0)
		leftRightRotate(root);
	else if (bf < -1 && cmpKeys(key, (*root)->right->key)< 0)
		rightLeftRotate(root);
}

void avlRemove(avlNode **root, const void *key, int (*cmpKeys)(const void *,const void *), void (*freeKey)(void *), void (*freeData)(void *))
{
	if (*root == NULL) {
		return;
	} else if (cmpKeys(key, (*root)->key) > 0) {
		avlRemove(&((*root)->right), key, cmpKeys, freeKey, freeData);
		return;
	} else if (cmpKeys(key, (*root)->key) < 0) {
		avlRemove(&((*root)->left), key, cmpKeys, freeKey, freeData);
		return;
	} else {
		if (freeKey)
			freeKey((*root)->key);

		if (freeData)
			freeData((*root)->data);

		if ((*root)->left == NULL) {
			avlNode *tmp = (*root)->right;
			free(*root);
			*root = tmp;
			return;
		} else if ((*root)->right == NULL) {
			avlNode *tmp = (*root)->left;
			free(*root);
			*root = tmp;
			return;
		}

		avlNode *succesor = avlGetMin((*root)->right);

		(*root)->key = succesor->key;
		(*root)->data = succesor->data;

		avlRemove(&((*root)->right), succesor->key, cmpKeys, NULL, NULL);
	}

	updateHeight(*root);
	int bf = getBalanceFactor(*root);

	if (bf > 1 && getBalanceFactor((*root)->left) >= 0)
		rightRotate(root);
	else if (bf < -1 && getBalanceFactor((*root)->right) <= 0)
		leftRotate(root);
	else if (bf > 1 && getBalanceFactor((*root)->left) < 0)
		leftRightRotate(root);
	else if (bf < -1 && getBalanceFactor((*root)->right) > 0)
		rightLeftRotate(root);
}

void avlUpdate(avlNode *root, const void *key, int cmpKeys(const void *,const void *), void freeData(void *), void *newData)
{
	avlNode *node = (avlNode *)avlFindNode(root, key, cmpKeys);

	if (node == NULL)
		return;

	if (freeData)
		freeData(node->data);

	node->data = newData;
}

void avlOrderTraverse(const avlNode *root, void action(const void *key, const void *data, void *arg), void *arg)
{
	if (root == NULL)
		return;

    avlOrderTraverse(root->left, action, arg);
    action(root->key, root->data, arg);
    avlOrderTraverse(root->right, action, arg);
}

void printCharNTimes(const char c, int n)
{
	for (int i = 0; i < n; i++)
		putchar(c);
}

void avlPrint(const avlNode *root, void print(const void *key, const void *data, void *arg), void *arg, int padding)
{
	if (root == NULL)
		return;

	printCharNTimes(' ', padding);
	print(root->key, root->data, arg);
	avlPrint(root->right, print, arg, padding + 4);
	avlPrint(root->left, print, arg, padding + 4);
}

void avlFree(avlNode *root, void freeKey(void *), void freeData(void *))
{
	if (root == NULL)
		return;

	if (freeKey != NULL)
		freeKey(root->key);

	if (freeData != NULL)
		freeData(root->data);

	avlFree(root->left, freeKey, freeData);
	avlFree(root->right, freeKey, freeData);

	free(root);
}

/*********************************************


             Map IMPLEMENTATION


**********************************************/

Map *newMap(int cmpKeys(const void *, const void *))
{
	Map *m = malloc(sizeof(Map));
	m->cmpKeys = cmpKeys;
	m->root = NULL;

	return m;
}

void mapInsert(Map *m, void *key, void *data)
{
	avlInsert(&m->root, key, data, m->cmpKeys);
}

void *mapFind(const Map *m, const void *key)
{
	return avlFind(m->root, key, m->cmpKeys);
}

bool mapIsExists(const Map *m, const void *key)
{
	return avlIsExists(m->root, key, m->cmpKeys);
}

void mapRemove(Map *m, const void *key, void freeKey(void *), void freeData(void *))
{
	avlRemove(&m->root, key, m->cmpKeys, freeKey, freeData);
}

void mapUpdate(Map *m, const void *key, void freeData(void *), void *newData)
{
	avlUpdate(m->root, key, m->cmpKeys, freeData, newData);
}

void mapOrderTraverse(const Map *m, void action(const void *key, const void *data, void *arg), void *arg)
{
    avlOrderTraverse(m->root, action, arg);
}

bool mapIsEmpty(const Map *m)
{
    return m->root == NULL;
}

void mapFree(Map *m, void freeKey(void *), void freeData(void *))
{
	avlFree(m->root, freeKey, freeData);
}

/*********************************************


             Cursor IMPLEMENTATION


**********************************************/
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

static struct termios originalTermios;

Result enableRawMode(int vminKeys, int vtime)
{
	if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) {
		return Err;
	}

	struct termios raw = originalTermios;
	raw.c_cc[VMIN] = vminKeys;
	raw.c_cc[VTIME] = vtime;
	raw.c_cflag |= (CS8);
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(IXON | ICRNL | ISTRIP | INPCK | BRKINT);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		return Err;
	}

	return Ok;
}

Result disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1) {
		return Err;
	}

	return Ok;
}

void disableLineWrap()
{
	printf("\033[?7l");
}

void enableLineWrap()
{
	printf("\033[?7h");
}

void hideCursor()
{
	printf("\033[?25l");
}

void showCursor()
{
	printf("\033[?25h");
}

void setCursorStyle(CURSOR_STYLE style)
{
	printf("\033[%d q", (int)style);
}

Result getCursorPos(int *x, int *y)
{
	printf("\033[6n");

	if (scanf("\033[%d;%dR", y, x) == 2) {
		return Ok;
	}

	return Err;
}

void setCursorPos(int x, int y)
{
	printf("\033[%d;%dH", y ,x);
}

void setCursorX(int x)
{
	printf("\033[%dG", x);
}

void cursorUp(int n)
{
	printf("\033[%dA", n);
}

void cursorDown(int n)
{
	printf("\033[%dB", n);
}

void cursorRight(int n)
{
	printf("\033[%dC", n);
}

void cursorLeft(int n)
{
	printf("\033[%dD", n);
}

void saveCursorPos()
{
	printf("\033[s");
}

void restoreCursorPos()
{
	printf("\033[u");
}

void enterAlternativeScreen()
{
	printf("\033[?1049h");
}

void exitAlternativeScreen()
{
	printf("\033[?1049l");
}

void clearLine()
{
	printf("\033[K");
}

void clearScreen()
{
	printf("\033[2J");
}

void updateScreen()
{
	fflush(stdout);
}

Result getScreenSizeByCursor(int *width, int *height)
{
	setCursorPos(999, 999);

	return getCursorPos(width, height);
}

Result getScreenSizeByIoctl(int *width, int *height)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
		return Err;
	}

	*width = ws.ws_col;
	*height = ws.ws_row;

	return Ok;
}

Result getScreenSize(int *width, int *height)
{
	if (getScreenSizeByIoctl(width, height) == Ok) {
		return Ok;
	}

	return getScreenSizeByCursor(width, height);
}

Result registerChangeInWindowSize(void funciton(int))
{
	struct sigaction sa;
	sa.sa_handler = funciton;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGWINCH, &sa, NULL) == -1) {
		return Err;
	}

	return Ok;
}

Result enableFullBuffering(FILE *fp)
{
	// do not flush on '\n'
	if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0) {
		return Err;
	}

	return Ok;
}

int waitForByte()
{
	char c;

	if (read(STDIN_FILENO, &c, 1) != 1) {
		return EMPTY_KEY;
	}

	return c;
}

#define SEQUENCE(a, b) ((unsigned short)((((unsigned short)b) << 8) | a))

int readEscapeKey()
{
	unsigned short key;

	if (read(STDIN_FILENO, &key, 2) != 2) {
		return '\033';
	}


	switch (key) {
		case SEQUENCE('[', 'A'): return ARROW_UP;
		case SEQUENCE('[', 'B'): return ARROW_DOWN;
		case SEQUENCE('[', 'C'): return ARROW_RIGHT;
		case SEQUENCE('[', 'D'): return ARROW_LEFT;
		case SEQUENCE('[', '1'): return HOME; // might be with a ~
		case SEQUENCE('[', '5'): return PAGE_UP; // might be with a ~
		case SEQUENCE('[', '6'): return PAGE_DOWN; // might be with a ~
	}

	return '\033';
}

int readKey()
{
	char c = waitForByte();

	if (c == '\033') {
		return readEscapeKey();
	}

	return c;
}

/*********************************************


             Path IMPLEMENTATION


**********************************************/

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

const char *getPathExtention(const char *path)
{
	const char *lastDot = strrchr(path, '.');

	if (lastDot == NULL) {
		return path;
	}

	return lastDot + 1;
}

const char *getHomePath()
{
	const char *home = getenv("HOME");

	if (home == NULL) {
		return ".";
	}

	return home;
}

char *expandPath(const char *path)
{
	if (path[0] == '~') {

		const char *home = getHomePath();
		int size = getFmtSize("%s%s", home, path + 1) + 1;
		char *dst = malloc(sizeof(char) * size);
		snprintf(dst, size, "%s%s", home, path + 1);

		return dst;
	}

	return strdup(path);
}

void compressPath(char *path)
{
	const char *home = getHomePath();
	int homeLen = strlen(home);

	if (strncmp(home, path, homeLen) == 0) {
		int bufLen = strlen(path) + homeLen;
		char buf[bufLen];
		snprintf(buf, bufLen, "~%s", path + homeLen);
		strncpy(path, buf, bufLen);
	}
}

Result nextInDir(DIR *dir, const char *dirName, char *destFileName, int destLen)
{
	struct dirent *de = readdir(dir);

	if (de == NULL) {
		return Err;
	}

	snprintf(destFileName, destLen, "%s/%s", dirName, de->d_name);

	return Ok;
}

Result dirTraverse(const char *dir, bool action(const char *))
{
	struct dirent *de;
	DIR *dr = opendir(dir);

	if (dr == NULL) {
		return Err;
	}

	while ((de = readdir(dr))) {

		const char *file = de->d_name;
		char fullPath[PATH_MAX];

		int len = snprintf(fullPath, PATH_MAX, "%s/%s", dir, file);
		fullPath[len] = '\0';

		if (action(fullPath) == false) {
			break;
		}
	}

	closedir(dr);

	return Ok;
}

bool isExtentionEqual(const char *path, const char *extention)
{
	return strcmp(getPathExtention(path), extention) == 0;
}

bool isDir(const char *path)
{
	struct stat sb;
	stat(path, &sb);

	return S_ISDIR(sb.st_mode);
}

bool isRegularFile(const char *path)
{
	struct stat sb;
	stat(path, &sb);

	return S_ISREG(sb.st_mode);
}

bool isPathExists(const char *path)
{
	return !access(path, F_OK);
}

Result echoFileWrite(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "w");

	if (fp == NULL) {
		return Err;
	}

	vfprintf(fp, fmt, ap);
	va_end(ap);
	return Ok;
}

Result echoFileAppend(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "a");

	if (fp == NULL) {
		return Err;
	}

	vfprintf(fp, fmt, ap);
	va_end(ap);
	return Ok;
}

Result readFile(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		return Err;
	}

	if (vfscanf(fp, fmt, ap) == EOF) {
        fclose(fp);
		return Err;
    }

	va_end(ap);
    fclose(fp);
	return Ok;
}

Result redirectFd(int srcFd, const char *destFileName)
{
	int destFd = open(destFileName, O_WRONLY);

	if (destFd == -1) {
		return Err;
	}

	if (dup2(destFd, srcFd) == -1) {
		close(destFd);
		return Err;
	}

	close(destFd);

	return Ok;
}

Result traverseFile(const char *fileName, int bufSize, bool action(char[bufSize]))
{
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		return Err;
	}

	char buf[bufSize];
	while (fgets(buf, bufSize, fp) && action(buf));

	return Ok;
}

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen)
{
	snprintf(dest, destLen, "%s/%s", dirName, fileName);
}

Result popen2(char *path, char *argv[], FILE *ppipe[2])
{
	int output[2];
	int input[2];

	if (pipe(output) == -1 || pipe(input) == -1) {
		return Err;
	}

	int pid = fork();

	if (pid == -1) {
		return Err;
	}

	if (pid) {
		// parent
		close(output[Write]);
		ppipe[Write] = fdopen(input[Write], "w");
		ppipe[Read] = fdopen(output[Read], "r");
	} else {
		// child
		dup2(input[Read], STDIN_FILENO);
		dup2(output[Write], STDOUT_FILENO);
		close(input[Write]);
		close(input[Read]);
		close(output[Write]);
		close(output[Read]);
		execvp(path, argv);
		exit(EXIT_FAILURE);
	}

	return Ok;
}

/*********************************************


              String IMPLEMENTATION


**********************************************/


void swap(void *a, void *b, const size_t size)
{
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}

size_t getFileSize(FILE *fp)
{
	const size_t curr = ftell(fp);
	fseek(fp, 0, SEEK_END);

	const size_t size = ftell(fp);
	fseek(fp, curr, SEEK_SET);

	return size;
}

int getFmtSize(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int size = getFmtSizeVa(fmt, ap);
	va_end(ap);

	return size;
}

int getFmtSizeVa(const char *fmt, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);

	int size = vsnprintf(NULL, 0, fmt, ap1);
	va_end(ap1);

	return size;
}

void *memdup(const void *mem, const size_t size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}

int zatarMax(int a, int b)
{
	return a > b ? a : b;
}

int zatarMin(int a, int b)
{
	return a > b ? b : a;
}

int zatarMin3(int a, int b, int c)
{
	return zatarMin(a, zatarMin(b, c));
}

int zatarMax3(int a, int b, int c)
{
	return zatarMax(a, zatarMax(b, c));
}


/*============================================


             -End-Implementation-


=============================================*/
#endif // LIBZATAR_IMPL
#endif // LIBZATRAR_h
