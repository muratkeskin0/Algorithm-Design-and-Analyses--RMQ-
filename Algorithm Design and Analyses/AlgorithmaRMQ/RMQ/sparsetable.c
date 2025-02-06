#include "sparsetable.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int **sparse_table;
int *log_table;

void build_sparse_table(int *array, int n) {

    int k = log2(n) + 1;
    sparse_table = malloc(n * sizeof(int*));
    if (sparse_table == NULL) {
        fprintf(stderr, "Memory allocation failed for sparse_table.\n");
        exit(EXIT_FAILURE);
    }

    log_table = malloc((n+1) * sizeof(int));
    if (log_table == NULL) {
        fprintf(stderr, "Memory allocation failed for log_table.\n");
        exit(EXIT_FAILURE);
    }

    log_table[0] = 0;
    log_table[1] = 0;
    for (int i = 2; i <= n; i++) {
        log_table[i] = log_table[i / 2] + 1;
    }

    for (int i = 0; i < n; i++) {
        sparse_table[i] = malloc(k * sizeof(int));
        if (sparse_table[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for sparse_table[%d].\n", i);
            exit(EXIT_FAILURE);
        }
        sparse_table[i][0] = array[i];
    }

    for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 0; i + (1 << j) - 1 < n; i++) {
            sparse_table[i][j] = (sparse_table[i][j-1] < sparse_table[i + (1 << (j-1))][j-1]) ?
                                sparse_table[i][j-1] : sparse_table[i + (1 << (j-1))][j-1];
        }
    }
}

int query_sparse(int i, int j) {
    int len = j - i + 1;
    int k = log_table[len];
    return sparse_table[i][k] < sparse_table[j - (1 << k) + 1][k] ?
           sparse_table[i][k] : sparse_table[j - (1 << k) + 1][k];
}

void run_sparse_table_separate_times(int *array, int n, int queries) {
    // 1) Measure Preprocessing
    double start = get_time();
    build_sparse_table(array, n);
    double mid = get_time();
    double build_time = mid - start;

    // 2) Measure Queries
    for (int q = 0; q < queries; q++) {
        int i = rand() % n;
        int j = i + rand() % (n - i);
        query_sparse(i, j);
    }
    double end = get_time();
    double query_time = end - mid;

    // Print and/or log
    printf("[SparseTable] n=%d, q=%d | build=%.6f sec, query=%.6f sec\n",
           n, queries, build_time, query_time);
    // Optional CSV logging:
    log_result("sparse_table_build.csv", n, queries, build_time);
    log_result("sparse_table_query.csv", n, queries, query_time);

    // Cleanup memory
    for (int i = 0; i < n; i++) {
        free(sparse_table[i]);
    }
    free(sparse_table);
    free(log_table);
}

