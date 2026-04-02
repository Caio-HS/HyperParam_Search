#include <fann.h>
#include <errno.h>

#include "data.h"

static int prepare_data(struct fann_train_data * restrict const data);

int get_train_data(struct fann_train_data ** restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    int error_code = 0;

    struct fann_train_data * new_data = fann_read_train_from_file(filename);
    if(*data == NULL) {return ENOMEM;}
    
    const unsigned int data_size = fann_length_train_data(new_data);
    
    *data = fann_subset_train_data(new_data, 0, (unsigned int) (((float)data_size) * DATA_USED_IN_TRAIN));
    fann_destroy_train(new_data);
    new_data = NULL;

    error_code = prepare_data(*data);
    if(error_code != 0)
    {
        fann_destroy_train(*data);
        *data = NULL;
        return error_code;
    }
    
    return 0;
}

int get_test_data(struct fann_train_data ** restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    int error_code = 0;

    struct fann_train_data * new_data = fann_read_train_from_file(filename);
    if(*data == NULL) {return ENOMEM;}
    
    const unsigned int data_size = fann_length_train_data(new_data);
    
    *data = fann_subset_train_data(new_data,(unsigned int) (((float)data_size) * DATA_USED_IN_TRAIN + 1), data_size - 1);
    fann_destroy_train(new_data);
    new_data = NULL;

    error_code = prepare_data(*data);
    if(error_code != 0)
    {
        fann_destroy_train(*data);
        *data = NULL;
        return error_code;
    }
    
    return 0;
}


static int prepare_data(struct fann_train_data * restrict const data)
{
    if (data == NULL) { return EINVAL; }
    
    fann_shuffle_train_data(data);
    fann_scale_train_data(data, MIN_SCALE, MAX_SCALE);

    return 0;
}
