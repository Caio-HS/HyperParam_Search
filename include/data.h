#ifndef DATA
#define DATA

#include <fann.h>

#define DATA_FILENAME "./data/energy_efficiency.data"

int get_train_data(struct fann_train_data ** restrict const data, const char * restrict const filename);
int get_test_data(struct fann_train_data ** restrict const data, const char * restrict const filename);


#endif
