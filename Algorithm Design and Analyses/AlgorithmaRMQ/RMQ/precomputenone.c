#include "precomputenone.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int rmq_no_precompute(int *array, int i, int j) {
    int min = array[i];
    for (int k = i; k <= j; k++) {
        if (array[k] < min) {
            min = array[k];
        }
    }
    return min;
}

void run_no_precompute_separate_times(int *array, int n, int queries) {
    // 1) "Build time" (naive has essentially no preprocessing)
    double start = get_time();
    // No actual preprocessing
    double mid = get_time();
    double build_time = mid - start;

    // 2) Query time
    for (int q = 0; q < queries; q++) {
        int i = rand() % n;
        int j = i + rand() % (n - i);
        rmq_no_precompute(array, i, j);
    }
    double end = get_time();
    double query_time = end - mid;

    // 3) Print or log the separate times
    printf("[NaiveRMQ] n=%d, q=%d | build=%.6f sec, query=%.6f sec\n",
           n, queries, build_time, query_time);

    // Optional: log to CSV files for each phase if desired
    log_result("no_precompute_build.csv", n, queries, build_time);
    log_result("no_precompute_query.csv", n, queries, query_time);
}
