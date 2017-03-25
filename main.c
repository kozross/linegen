#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bstrlib/bstrlib.h"

#define SCR_W 75
#define SCR_H 25

#define B_SIZE 32

typedef char scr_t[SCR_H][SCR_W + 1];

typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    point_t p1;
    point_t p2;

    point_t pd; // difference
} line_t;

void init_scr(scr_t scr) {
    for (size_t i = 0; i < SCR_H; i++) {
        memset(scr[i], '-', SCR_W);

        scr[i][SCR_W] = '\0';
    }
}

void print_scr(scr_t scr) {
    for (size_t i = 0; i < SCR_H; i++) {
        printf("%s\n", scr[i]);
    }
}

void append_scr(scr_t scr, line_t line) {
    assert(!(line.p1.x < 0 || line.p1.x > SCR_W)); // out of x range
    assert(!(line.p2.x< 0 || line.p2.x > SCR_W)); // out of x range
    assert(!(line.p1.y < 0 || line.p1.y > SCR_H)); // out of y range
    assert(!(line.p2.y < 0 || line.p2.y > SCR_H)); // out of y range

    for (int x = line.p1.x; x <= line.p2.x; x++) {
        int y = line.p1.y + line.pd.y * (x - line.p1.x) / line.pd.x;
        scr[y][x] = '*';
    }
}

void load_scr(char* fname, scr_t scr) {
    FILE* fptr = fopen(fname, "r");
    assert(fptr);

    bstring b = bread((bNread)fread, fptr);
    fclose(fptr);

    struct bstrList *lines = bsplit(b, '\n');

    for (int i = 0; i < lines->qty; i++) {
        struct bstrList *points = bsplit(lines->entry[i], ' ');
        line_t line;

        int j;
        for (j = 0; j < points->qty; j++) {
            if (j == 0) {
                assert(!(strcmp("line", bdata(points->entry[j])))); // unknown identifier
                continue;
            }
                    
            assert(!(j > 5)); // excess arguments

            char* endptr;
            int n = strtol(bdata(points->entry[j]), &endptr, 10);

            if (j == 1) {
                line.p1.x = n;
            } else if (j == 2) {
                line.p1.y = n;
            } else if (j == 3) {
                line.p2.x = n;
            } else if (j == 4) {
                line.p2.y = n;
            }
        }

        assert(!(j != 5)); // missing arguments

        line.pd.x = line.p2.x - line.p1.x;
        line.pd.y = line.p2.y - line.p1.y;
        
        // the program just dies somewhere around here

        assert(!(line.pd.x == 0 || line.pd.y == 0)); // invalid line

        append_scr(scr, line);
    }
}

int main(int argc, char **argv) {
    if (!argv[1] || strcmp(argv[1], "help") == 0) {
        printf("usage:\n    linegen <path-to-file>\n\n");
        exit(1);
    }

    scr_t scr;

    init_scr(scr);
    load_scr(argv[1], scr);
    print_scr(scr);

    getchar();
    return 0;
}
