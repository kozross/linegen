#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

#define SCR_W 80
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
        printf("%s", scr[i]);
    }
}

void append_scr(scr_t scr, line_t line) {
    if (line.p1.x < 0 || line.p1.x > SCR_W) {
        printf("error: invalid arg '%d'; out of x range (0 - %d)\n",
            line.p1.x, SCR_W);
        exit(1);
    }

    if (line.p2.x< 0 || line.p2.x > SCR_W) {
        printf("error: invalid arg '%d'; out of x range (0 - %d)\n",
            line.p2.x, SCR_W);
        exit(1);
    }

    if (line.p1.y < 0 || line.p1.y > SCR_H) {
        printf("error: invalid arg '%d'; out of y range (0 - %d)\n",
            line.p2.x, SCR_H);
        exit(1);
    }

    if (line.p2.y < 0 || line.p2.y > SCR_H) {
        printf("error: invalid arg '%d'; out of y range (0 - %d)\n",
            line.p2.x, SCR_H);
        exit(1);
    }

    for (int x = line.p1.x; x <= line.p2.x; x++) {
        int y = line.p1.y + line.pd.y * (x - line.p1.x) / line.pd.x;
        scr[y][x] = '*';
    }
}

char* get_line(FILE* fptr, line_t* lptr) {
    static char line[B_SIZE];
    static char buff[B_SIZE];

    char* str = fgets(line, B_SIZE, fptr);
    if (!str) {
        return str;
    }
    
    strcpy(buff, line);

    char* bptr = buff;
    char* tok;

    int i;
    for (i = 0; tok = strtok_r(bptr, " ", &bptr); i++) {
        if (i == 0) {
            if(strcmp(tok, "line") == 0) continue;
            printf("error: invalid identifier '%s'; did you mean 'line'?\n", tok);
            exit(1);
        }

        if (i > 4) {
            printf("error: invalid line '%s'; excess arg(s)\n", line);
            exit(1);
        }

        int n = atoi(tok);
        if (n == 0 && tok[0] != '0') {
            printf("error: invalid arg '%s'; expected type 'int'\n", tok);
            exit(1);
        }

        *(int*[4]) { &lptr->p1.x, &lptr->p1.y,
                &lptr->p2.x, &lptr->p2.y }[i - 1] = n;
    }

    if (i < 5) {
        printf("error: invalid line '%s'; missing arg(s)\n", line);
        exit(1);
    }

    return str;
}

int main(int argc, char** argv) {
    if (!argv[1] || strcmp(argv[1], "help") == 0) {
        printf("usage:\n    linegen <path-to-file>\n\n");
        exit(1);
    }

    FILE* fptr = fopen(argv[1], "r");
    if (!fptr) {
        printf("error: file not found\n");
        exit(1);
    }

    scr_t scr;
    init_scr(scr);

    line_t line;
    while (get_line(fptr, &line)) {
        line.pd.x = line.p2.x - line.p1.x;
        line.pd.y = line.p2.y - line.p1.y;

        if (line.pd.x == 0 || line.pd.y == 0) {
            printf("error: invalid line '%d %d %d %d'; difference amounts to 0\n",
                line.p1.x, line.p1.y, line.p2.x, line.p2.y);
            exit(1);
        }

        append_scr(scr, line);
    }

    print_scr(scr);

    fclose(fptr);
    return 0;
}
