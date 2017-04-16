#include "scr.h"

Scr *scr_new(size_t width, size_t height) {
    Scr *scr = malloc(sizeof(Scr) + (sizeof(char*) * height));
    if(!scr) return scr;

    scr->width = width;
    scr->height = height;

    for (size_t r = 0; r < height; r++) {
        if((scr->map[r] = malloc(width + 1))) {
            memset(scr->map[r], '-', width);
            scr->map[r][width] = '\0';
        }
    }
    
    return scr;
}

void scr_draw_point(Scr *scr, Point point, char c) {
    scr->map[point.y][point.x] = c;
}

void scr_draw_points(Scr *scr, Points *points, char c) {
    for (size_t i = 0; i < points->len; i++) {
        scr_draw_point(scr, points->data[i], c);
    }
}

void scr_print(Scr *scr) {
    for (size_t r = 0; r < scr->height; r++) {
        printf("%s\n", scr->map[r]);
    }
}