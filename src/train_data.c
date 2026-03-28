#include <fann.h>
#include <errno.h>

#include "train_data.h"

int train_data(struct fann_train_data * restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    *data = fann_read_train_from_file(filename);
    
    return 0;
}
