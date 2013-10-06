#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

void dump_memory_map(void);

int main(int argc, char **argv) {
    void *m1 = malloc(63);
    void *m2 = malloc(100);
    free(m1);
    dump_memory_map();
    void *m3 = malloc(64);
    void *m4 = malloc(11);
    free(m3);
    void *m5 = malloc(10);
    void *m6 = malloc(120);
    free(m2);
    return 0;
}
