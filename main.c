#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"

#include "scr.h"

int main(int argc, char **argv) {
    if (argc == 1 || strcmp(argv[1], "help") == 0) {
        printf("usage:\n\tlinegen <path-to-file>\n\n");
        exit(1);
    }

    Scr *scr = scr_new(75, 25);

    Lines *lines = lines_from_file(
    	"lines.txt",
    	point_new(scr->width, scr->height)
    );

    for (size_t l = 0; l < lines->len; l++) {
    	Points *line_points = points_from_line(lines->data[l]);
    	scr_draw_points(scr, line_points, '*');

    	free(line_points);
    }

    scr_print(scr);

    free(lines);
    free(scr);

    return 0;
}