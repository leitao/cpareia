#ifndef _MEM_H_
#define _MEM_H_

#include <stdlib.h>
#include <stdio.h>

#include "errors.h"

typedef struct mem {
    unsigned long size, resident, share, text, lib, data, dt;
} mem;

void mem_print();

#endif
