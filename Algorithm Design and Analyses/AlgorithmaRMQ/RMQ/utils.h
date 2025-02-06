#ifndef UTILS_H
#define UTILS_H

// Logs experimental results to a CSV file
void log_result(const char *filename, int n, int queries, double time);

// Returns the current time in seconds
double get_time();

#endif
