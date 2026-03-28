#include <fann.h>
#include <errno.h>

#include "data.h"

int get_train_data(struct fann_train_data * restrict const * restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    *data = fann_read_train_from_file(filename);
    
    return 0;
}
