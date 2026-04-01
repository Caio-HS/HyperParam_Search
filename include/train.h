#ifndef TRAIN
#define TRAIN

#include <stdint.h>

#include "result.h"

#define TARGET_ERROR 0.005f
#define MAX_EPOCHS 200

int train_network(struct fann * restrict const ann, struct fann_train_data * restrict const data, RESULTS * restrict const results);

#endif
