#include "line.h"

void error(char* type, char *msg, char *src, size_t n) {
	if (msg) {
		if (n) {
			printf("%s:%u: %s-error: %s", src, n, type, msg);
			exit(1);
		}

		printf("%s: %s-error: %s", src, type, msg);
		exit(1);
	}

	printf("%s-error: %s", type, msg);
	exit(1);
}

void line_check(Line line, Point size, char *src, size_t n) {
	if (line.pd.x == 0) error("line", "x diff = 0", src, n);
	if (line.pd.y == 0) error("line", "y diff = 0", src, n);

	if (line.p1.x < 0) error("line", "x1 < 0", src, n);
	if (line.p2.x < 0) error("line", "x2 < 0", src, n);
	if (line.p1.y < 0) error("line", "y1 < 0", src, n);
	if (line.p2.y < 0) error("line", "y2 < 0", src, n);

	if (line.p1.x >= size.x) error("line", "x1 >= width", src, n);
	if (line.p2.x >= size.x) error("line", "x2 >= width", src, n);
	if (line.p1.y >= size.y) error("line", "y1 >= height", src, n);
	if (line.p2.y >= size.y) error("line", "y2 >= height", src, n);
}

Point point_new(size_t x, size_t y) {
	return (Point) {
		.x = x,
		.y = y,
	};
}

Line line_new(int x1, int y1, int x2, int y2) {
	return (Line) {
		.p1 = point_new(x1, y1),
		.p2 = point_new(x2, y2),
		.pd = point_new(x2 - x1, y2 - y1)
	};
}

Points *points_new(size_t len) {
	Points *points = malloc(sizeof(Points) + (sizeof(Point) * len));
	points->len = len;

	return points;
}

Lines *lines_new(size_t len) {
	Lines *lines = malloc(sizeof(Lines) + (sizeof(Line) * len));
	lines->len = len;

	return lines;
}

Points *points_from_line(Line line) {
	Points *points = points_new((line.p2.x - line.p1.x) + 1);

	for (int x = line.p1.x; x <= line.p2.x; x++) {
        int y = line.p1.y + line.pd.y * (x - line.p1.x) / line.pd.x;

        points->data[x - line.p1.x] = point_new(x, y);
    }

    return points;
}

Lines *lines_from_file(char *file_name, Point size) {
	FILE *file = fopen(file_name, "r");
	if (!file ) error("file", "cannot be opened", file_name, 0);

    bstring b = bread((bNread)fread, file);
    fclose(file);

    struct bstrList *file_lines = bsplit(b, '\n');

    Lines *lines = lines_new(file_lines->qty);

    for (int l = 0; l < file_lines->qty; l++) {
    	struct bstrList *args = bsplit(file_lines->entry[l], ' ');
    	if (args->entry[args->qty - 1]->slen == 0) args->qty--;

    	Line line;

    	int a;
    	for (a = 0; a < args->qty; a++) {
    		if (a == 0) {
    			bstring type = bfromcstr("line");

    			if (bstrcmp(type, args->entry[a]) != 0) {
    				bdestroy(type);
    				error("line", "unknown type", file_name, l + 1);
    			}

    			continue;
    		}

    		if (a > 5) error("line", "excess arguments", file_name, l + 1);

    		char* endptr;
            int n = strtol(bdata(args->entry[a]), &endptr, 10);

            if (a == 1) line.p1.x = n;
            else if (a == 2) line.p1.y = n;
            else if (a == 3) line.p2.x = n;
            else if (a == 4) line.p2.y = n;
    	}
    	if (a != 5) error("line", "missing arguments", file_name, l + 1);

    	lines->data[l] = line_new(line.p1.x, line.p1.y, line.p2.x, line.p2.y);

    	line_check(lines->data[l], size, file_name, l + 1);

    	bstrListDestroy(args);
    }

    bdestroy(b);
    bstrListDestroy(file_lines);

    return lines;
}