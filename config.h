#ifndef CONFIG_H
#define CONFIG_H

#include "cursor.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

const int avgStrideLen = 8;
const char exitKeys[] = "q\e";
const int delayDeciSeconds = 5; // 0 - 9, Default 5

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
