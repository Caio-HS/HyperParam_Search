#ifndef TRAIN_DATA
#define TRAIN_DATA

#include <fann.h>

#define DATA_FILENAME "./data/energy_efficiency.data"

int train_data(struct fann_train_data * restrict const data, const char * restrict const filename);

#endif
