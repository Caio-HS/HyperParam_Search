#include <fann.h>
#include <time.h>
#include <stdint.h>

#include "train.h"
#include "result.h"

#define NO_REPORTS 0

int train_network(struct fann * restrict const ann, struct fann_train_data * restrict const data)
{
    fann_train_on_data(ann, data, MAX_EPOCHS, NO_REPORTS, TARGET_ERROR);
    
    return 0;
}
