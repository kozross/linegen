#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point p1;
    Point p2;

    Point pd;
} Line;

typedef struct {
    size_t len;

    Point data[];
} Points;

typedef struct {
    size_t len;

    Line data[];
} Lines;

void error(char* type, char *msg, char *src, size_t n);
void line_check(Line line, Point size, char *src, size_t n);
Point point_new(size_t x, size_t y);
Line line_new(int x1, int y1, int x2, int y2);
Points *points_new(size_t len);
Lines *lines_new(size_t len);
Points *points_from_line(Line line);
Lines *lines_from_file(char *file_name, Point size);

#endif // LINE_H