#ifndef SCR_H
#define SCR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "line.h"

typedef struct {
    size_t width;
    size_t height;
    
    char *map[];
} Scr;

Scr *scr_new(size_t width, size_t height);
void scr_draw_point(Scr *scr, Point point, char c);
void scr_draw_points(Scr *scr, Points *points, char c);
void scr_print(Scr *scr);
void scr_free(Scr *scr);

#endif // SCR_H