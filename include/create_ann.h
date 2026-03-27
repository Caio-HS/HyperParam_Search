#ifndef CREATE_ANN
#define CREATE_ANN

#include "get_params.h"

#define MIN_WEIGHT -0.2f
#define MAX_WEIGHT 0.2f
#define SPARSE_THERESHOLD 0.005F

int create_configured_ann(struct fann ** restrict const ann, const PARAMETERS params);

#endif
