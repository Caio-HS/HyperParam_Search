#ifndef DATA
#define DATA

#include <fann.h>

#define DATA_FILENAME "./data/energy_efficiency.data"
#define DATA_USED_IN_TRAIN 0.750f
#define MIN_SCALE -1.0f
#define MAX_SCALE 1.0f

int get_train_data(struct fann_train_data ** restrict const data, const char * restrict const filename);
int get_test_data(struct fann_train_data ** restrict const data, const char * restrict const filename);


#endif
