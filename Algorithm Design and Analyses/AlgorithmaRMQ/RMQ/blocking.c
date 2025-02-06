#include "blocking.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BLOCK_SIZE 4  // Adjustable block size

int *block_min;  // Store minimum values for each block
int num_blocks;

// Preprocess the array by dividing it into blocks and finding the minimum for each block
void preprocess_blocking(int *array, int n) {
    num_blocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
    block_min = malloc(num_blocks * sizeof(int));

    // Check if memory allocation failed
    if (block_min == NULL) {
        fprintf(stderr, "Memory allocation for block_min failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_blocks; i++) {
        int start = i * BLOCK_SIZE;
        int end = (start + BLOCK_SIZE < n) ? start + BLOCK_SIZE : n;
        block_min[i] = array[start];  // Initialize block minimum

        // Calculate the minimum value in each block
        for (int j = start; j < end; j++) {
            if (array[j] < block_min[i]) {
                block_min[i] = array[j];
            }
        }
    }
}

// Perform RMQ using block precomputation and partial array scanning
int query_blocking(int *array, int i, int j) {
    int min_value = array[i];
    int start_block = i / BLOCK_SIZE;
    int end_block = j / BLOCK_SIZE;

    if (start_block == end_block) {
        // Case when the entire range is within one block
        for (int k = i; k <= j; k++) {
            if (array[k] < min_value) {
                min_value = array[k];
            }
        }
    } else {
        // Scan from i to the end of the starting block
        for (int k = i; k < (start_block + 1) * BLOCK_SIZE; k++) {
            if (array[k] < min_value) {
                min_value = array[k];
            }
        }

        // Scan complete blocks in between
        for (int b = start_block + 1; b < end_block; b++) {
            if (block_min[b] < min_value) {
                min_value = block_min[b];
            }
        }

        // Scan from the beginning of the ending block to j
        for (int k = end_block * BLOCK_SIZE; k <= j; k++) {
            if (array[k] < min_value) {
                min_value = array[k];
            }
        }
    }
    return min_value;
}

// Run experiments with Blocking RMQ and log the results
void run_blocking_separate_times(int *array, int n, int queries) {
    // 1) Measure build (preprocessing) time
    double start = get_time();
    preprocess_blocking(array, n);  
    double mid = get_time();
    double build_time = mid - start;

    // 2) Measure query time
    for (int q = 0; q < queries; q++) {
        int i = rand() % n;
        int j = i + rand() % (n - i);
        query_blocking(array, i, j);
    }
    double end = get_time();
    double query_time = end - mid;

    // 3) Print or log the separate times
    printf("[Blocking] n=%d, q=%d | build=%.6f sec, query=%.6f sec\n",
           n, queries, build_time, query_time);

    // If you want to log to separate CSV files:
    log_result("blocking_build.csv", n, queries, build_time);
    log_result("blocking_query.csv", n, queries, query_time);

    // 4) Cleanup
    free(block_min);
}
