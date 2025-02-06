#include "precomputeall.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int** preprocess(int *array, int n) {
    int **table = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        table[i] = malloc(n * sizeof(int));
        for (int j = i; j < n; j++) {
            table[i][j] = (i == j) ? array[i] : (table[i][j-1] < array[j] ? table[i][j-1] : array[j]);
        }
    }
    return table;
}

int rmq(int **table, int i, int j) {
    return table[i][j];
}

void run_precompute_all_separate_times(int *array, int n, int queries) {
    // 1) Measure preprocessing/build time
    double start = get_time();
    int **min_table = preprocess(array, n); 
    double mid = get_time();
    double build_time = mid - start;

    // 2) Measure query time
    for (int q = 0; q < queries; q++) {
        int i = rand() % n;
        int j = i + rand() % (n - i);
        rmq(min_table, i, j);
    }
    double end = get_time();
    double query_time = end - mid;

    // 3) Print or log both times
    printf("[PrecomputeAll] n=%d, q=%d | build=%.6f sec, query=%.6f sec\n",
           n, queries, build_time, query_time);
    log_result("precompute_all_build.csv", n, queries, build_time);
    log_result("precompute_all_query.csv", n, queries, query_time);

    // Clean up
    for (int i = 0; i < n; i++) {
        free(min_table[i]);
    }
    free(min_table);
}
