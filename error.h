#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

typedef const char *ERROR;

const ERROR OK               = 0;

const ERROR FILE_NOT_FOUND   = "File not found";
const ERROR FILE_READ_ERROR  = "File read error";
const ERROR FILE_WRITE_ERROR = "File write error";
const ERROR SCANF_ERROR      = "Scanf input not as expected";

void printError(const char *label, ERROR e);
void pdie(const char *label, ERROR e);
void die(const char *fmt, ...);

#ifdef ERROR_IMPL

void printError(const char *label, ERROR e)
{
    fprintf(stderr, "%s: %s\n", label, e);
}

void pdie(const char *label, ERROR e)
{
	printError(label, e);
	exit(EXIT_FAILURE);
}

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

#endif

#endif
