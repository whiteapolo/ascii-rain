#ifndef CONFIG_H
#define CONFIG_H

#include "cursor.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

const int avgStrideLen = 8;
const char exitKeys[] = "q\e";     // Keys that will cause the program to exit
const int delayDeciSeconds = 5;    // Delay between updates (0-9, Default 5)
const char inActiveChars[] =  " ";
const int inActiveCharsLen = ARRAY_SIZE(inActiveChars) - 1;

const char activeChars[] = {
    "!\"#$%&'()*+,-./0123456789:;<=>?@"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
    "abcdefghijklmnopqrstuvwxyz{|}~"
};
const int activeCharsLen = ARRAY_SIZE(activeChars) - 1;

const char activeColor[] = C2;

#endif
