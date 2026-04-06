#ifndef RESULT
#define RESULT

#include <stdint.h>
#include <fann.h>

struct results
{
    uint64_t total_parameters;
    uint64_t train_time;
    float train_mse_error;
    uint64_t train_bit_error;
    uint64_t test_time;
    float test_mse_error;
    uint64_t test_bit_error;
    uint64_t data_points_needed; //How much data entries are needed to reach the target error
};

struct context
{
    uint64_t last_epoch;
};

typedef struct context CONTEXT;

typedef struct results RESULTS;

int instrument_ann(struct fann * restrict const ann);
int set_train_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results);
int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results);
int send_results(const RESULTS * restrict const results, const PARAMETERS * const restrict params, RESULTS * restrict const output)
#endif
