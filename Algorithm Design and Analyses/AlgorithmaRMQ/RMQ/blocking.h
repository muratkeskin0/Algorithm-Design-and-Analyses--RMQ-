#ifndef BLOCKING_H
#define BLOCKING_H

void preprocess_blocking(int *array, int n);
int query_blocking(int *array,int i, int j);
void run_blocking_separate_times(int *array, int n, int queries);

#endif
