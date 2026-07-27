#ifndef RESULT
#define RESULT

#include <stdint.h>
#include <fann.h>

#include "get_params.h"

#define HASHS_SEED 0
#define RESULTS_COMPATIBILITY_VERSION 1u

struct results
{
    uint32_t total_parameters;
    int64_t parameters_hash;
    uint32_t data_points_needed; //How much data entries are needed to reach the target error
    uint16_t version;
    int64_t train_time;
    float train_mse_error;
    uint32_t train_bit_error;
    int64_t test_time;
    float test_mse_error;
    uint32_t test_bit_error;
};

struct context
{
    uint32_t last_epoch;
};

typedef struct context CONTEXT;

typedef struct results RESULTS;

int instrument_ann(struct fann * restrict const ann);
int set_train_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results);
int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results);
int send_results(const RESULTS * restrict const results, const PARAMETERS * const restrict params);
int set_parameters_hash(const PARAMETERS * restrict const params, RESULTS * restrict const results);

#endif
