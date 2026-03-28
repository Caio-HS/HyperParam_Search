#ifndef TRAIN
#define TRAIN

#include <stdint.h>

#define TARGET_ERROR 0.005f


struct results
{
    uint64_t total_parameters;
    uint64_t train_time;
    unsigned float train_mse_error;
    uint64_t train_bit_error;
    uint64_t test_time;
    unsigned float test_mse_error;
    uint64_t test_bit_error;
    usigned float data_points_needed; //How much data entries are needed to reach the target error
}

typedef struct results RESULTS;

int train_network(struct fann * restrict const ann, const struct fann_train_data * restrict const data, RESULTS * restrict const results);

#endif
