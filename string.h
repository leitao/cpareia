#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib.h>
#include <stdio.h>

char *each_token(char *, char *, char, void *,
    void (*call_back)(char *, char *, void *));
#endif
