#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

#define SCR_W 80
#define SCR_H 25

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
        printf("%s", scr[i]);
    }
}

void append_scr(scr_t scr, line_t line) {
    for (int x = line.p1.x; x <= line.p2.x; x++) {
        int y = line.p1.y + line.pd.y * (x - line.p1.x) / line.pd.x;
        scr[y][x] = '*';
    }
}

inline void raise_error(char* msg, int status) {
    printf("error: %s\n", msg);
    exit(status);
}


char* get_line(FILE* fptr, line_t* lptr) {
    static char buff[32];
    char* str = fgets(buff, 32, fptr);
    if (!str) {
        return str;
    }

    char* bptr = buff;
    char* tok;

    int i;
    for (i = 0; tok = strtok_r(bptr, " \n", &bptr); i++) {
        if (i == 0) continue;

        int n = atoi(tok);
        if (n == 0 && tok[0] != '0') {
            raise_error("invalid value(s)", 1);
        }

        *(int* [4]){ &lptr->p1.x, &lptr->p1.y,
            &lptr->p2.x, &lptr->p2.y }[i - 1] = n;
    }

    if (i < 5) {
        raise_error("missing argument(s)", 1);
    }

    return str;
}

int main(int argc, char** argv) {
    FILE* fptr = fopen(argv[1], "r");
    if (!fptr) {
        raise_error("file not found", 1);
    }

    scr_t scr;
    init_scr(scr);

    line_t line;
    while (get_line(fptr, &line)) {
        line.pd.x = line.p2.x - line.p1.x;
        line.pd.y = line.p2.y - line.p1.y;

        if (line.pd.x == 0 || line.pd.y == 0) {
            raise_error("invalid line(s)", 1);
        }

        append_scr(scr, line);
    }

    fclose(fptr);

    print_scr(scr);

    return 0;
}