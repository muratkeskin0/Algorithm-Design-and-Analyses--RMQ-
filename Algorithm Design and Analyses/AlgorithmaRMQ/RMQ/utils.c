#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>

// Logs the results of experiments to a CSV file
void log_result(const char *filename, int n, int queries, double time) {
    FILE *fp = fopen(filename, "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%d,%f\n", n, queries, time);
        fclose(fp);
    } else {
        printf("Error opening file for logging.\n");
    }
}

// Returns the current time in seconds with better precision
double get_time() {
    LARGE_INTEGER freq, start;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    return (double)start.QuadPart / freq.QuadPart;
}

