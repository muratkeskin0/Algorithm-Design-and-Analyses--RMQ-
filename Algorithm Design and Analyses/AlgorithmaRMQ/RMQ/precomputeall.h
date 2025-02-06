#ifndef PRECOMPUTE_ALL_H
#define PRECOMPUTE_ALL_H

int** preprocess(int *array, int n);
int rmq(int **table, int i, int j);
void run_precompute_all_separate_times(int *array, int n, int queries);

#endif
