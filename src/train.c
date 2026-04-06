#include <fann.h>
#include <time.h>
#include <stdint.h>

#include "train.h"

int train_network(struct fann * restrict const ann, struct fann_train_data * restrict const data)
{
    fann_train_on_data(ann, data, MAX_EPOCHS, REPORT_RATE, TARGET_ERROR);
    
    return 0;
}
