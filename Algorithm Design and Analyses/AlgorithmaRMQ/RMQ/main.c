#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "precomputeall.h"  // run_precompute_all_separate_times(...)
#include "sparsetable.h"    // run_sparse_table_separate_times(...)
#include "blocking.h"       // run_blocking_separate_times(...)
#include "precomputenone.h" // run_no_precompute_separate_times(...)
#include "utils.h"

#define MAX_SIZE 10000

// Test array sizes and query counts
static int SIZES[] = {1000, 2000, 5000, 10000};
static int QUERIES[] = {100, 500, 1000};

// Utility function to fill arrays in different ways
void fill_random(int *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 1000;  // Random values [0..999]
    }
}

void fill_sorted(int *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = i;  // Ascending order
    }
}

void fill_reversed(int *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = n - i;  // Descending order
    }
}

// Function to run RMQ for fixed array size with different array types
void test_fixed_array_type(int n, int queries) {
    if (n > MAX_SIZE) return;

    // Allocate array
    int *array = (int *)malloc(n * sizeof(int));
    if (!array) {
        fprintf(stderr, "Memory allocation failed for n=%d.\n", n);
        return;
    }

    // Test with Random Array
    fill_random(array, n);
    printf("\n--- [Random, n=%d, q=%d] ---\n", n, queries);
    run_precompute_all_separate_times(array, n, queries);
    run_sparse_table_separate_times(array, n, queries);
    run_blocking_separate_times(array, n, queries);
    run_no_precompute_separate_times(array, n, queries);

    // Test with Sorted Array
    fill_sorted(array, n);
    printf("\n--- [Sorted, n=%d, q=%d] ---\n", n, queries);
    run_precompute_all_separate_times(array, n, queries);
    run_sparse_table_separate_times(array, n, queries);
    run_blocking_separate_times(array, n, queries);
    run_no_precompute_separate_times(array, n, queries);

    // Test with Reversed Array
    fill_reversed(array, n);
    printf("\n--- [Reversed, n=%d, q=%d] ---\n", n, queries);
    run_precompute_all_separate_times(array, n, queries);
    run_sparse_table_separate_times(array, n, queries);
    run_blocking_separate_times(array, n, queries);
    run_no_precompute_separate_times(array, n, queries);

    free(array);
}

// Function to run RMQ for fixed array size with varying queries
void test_fixed_array_size(int *array, int n, size_t queryCount) {
    for (size_t q = 0; q < queryCount; q++) {
        int queries = QUERIES[q];

        printf("\n--- [Fixed n=%d, Testing queries=%d] ---\n", n, queries);

        run_precompute_all_separate_times(array, n, queries);
        run_sparse_table_separate_times(array, n, queries);
        run_blocking_separate_times(array, n, queries);
        run_no_precompute_separate_times(array, n, queries);
    }
}

// Function to run RMQ for fixed queries with varying array sizes
void test_fixed_query_count(int queries, size_t sizeCount) {
    for (size_t i = 0; i < sizeCount; i++) {
        int n = SIZES[i];
        if (n > MAX_SIZE) continue;

        int *array = (int *)malloc(n * sizeof(int));
        if (!array) {
            fprintf(stderr, "Memory allocation failed for n=%d.\n", n);
            continue;
        }

        fill_random(array, n);

        printf("\n--- [Fixed queries=%d, Testing n=%d] ---\n", queries, n);

        run_precompute_all_separate_times(array, n, queries);
        run_sparse_table_separate_times(array, n, queries);
        run_blocking_separate_times(array, n, queries);
        run_no_precompute_separate_times(array, n, queries);

        free(array);
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    printf("=== RMQ Benchmarks (No User Prompts) ===\n");

    size_t sizeCount = sizeof(SIZES) / sizeof(SIZES[0]);
    size_t queryCount = sizeof(QUERIES) / sizeof(QUERIES[0]);

    // 1. Loop over fixed array sizes (varying queries)
    for (size_t i = 0; i < sizeCount; i++) {
        int n = SIZES[i];
        if (n > MAX_SIZE) continue;

        int *array = (int *)malloc(n * sizeof(int));
        if (!array) {
            fprintf(stderr, "Memory allocation failed for n=%d.\n", n);
            continue;
        }

        fill_random(array, n);
        printf("\n=== [Fixed Array Size: n=%d] ===\n", n);
        test_fixed_array_size(array, n, queryCount);
        free(array);
    }

    // 2. Loop over fixed query counts (varying array sizes)
    printf("\n=== [Fixed Query Testing Mode] ===\n");
    for (size_t q = 0; q < queryCount; q++) {
        int queries = QUERIES[q];
        test_fixed_query_count(queries, sizeCount);
    }

    // 3. Fixed array size and query count with varying array types
    printf("\n=== [Array Type Testing Mode] ===\n");
    for (size_t i = 0; i < sizeCount; i++) {
        int n = SIZES[i];
        if (n > MAX_SIZE) continue;

        for (size_t q = 0; q < queryCount; q++) {
            int queries = QUERIES[q];
            test_fixed_array_type(n, queries);
        }
    }

    printf("All tests completed.\n");
    return 0;
}
