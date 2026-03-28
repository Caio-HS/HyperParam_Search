#include <fann.h>
#include <time.h>
#include <stdint.h>

#include "train.h"

#define NO_REPORTS 0

int train_network(struct fann * restrict const ann, struct fann_train_data * restrict const data, RESULTS * restrict const results)
{
    clock_t inicio = clock();

    fann_train_on_data(ann, data, MAX_EPOCHS, NO_REPORTS, TARGET_ERROR);

    clock_t fim = clock();

    results->train_time = (uint64_t)(fim - inicio);
    
    return 0;
}
