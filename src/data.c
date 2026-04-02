#include <fann.h>
#include <stdbool.h>
#include <errno.h>

#include "data.h"

static int prepare_data(struct fann_train_data * restrict const data);
static int get_data_subset(struct fann_train_data ** restrict const output, const float percentage_of_dataset_used, const char * restrict const filename);

int get_train_data(struct fann_train_data ** restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    int error_code = 0;
    

    error_code = prepare_data(*data);
    if(error_code != 0) { return error_code; }
    
    return 0;
}

int get_test_data(struct fann_train_data ** restrict const data, const char * restrict const filename)
{
    if(data == NULL) { return EINVAL; }

    int error_code = 0;

    error_code = prepare_data(*data);
    if(error_code != 0) { return error_code; }
    
    return 0;
}


static int prepare_data(struct fann_train_data * restrict const data)
{
    if (data == NULL) { return EINVAL; }
    
    fann_shuffle_train_data(data);
    fann_scale_train_data(data, MIN_SCALE, MAX_SCALE);

    return 0;
}


static int get_data_subset(struct fann_train_data ** restrict const output, const float percentage_of_dataset_used, const bool isTest, const char * restrict const filename)
{
    if(output == NULL) { return EINVAL; }
    
    struct fann_train_data * input = fann_read_train_from_file(filename);
    if(*input == NULL) {return ENOMEM;}
    
    const unsigned int data_size = fann_length_train_data(input);

    unsigned int init;
    unsigned int end;
    unsigned int mean = (unsigned int) ( ( (float) data_size ) * percentage_of_dataset_used );

    if(isTest)
    {
        init = (unsigned int) ( (float) data_size ) * DATA_USED_IN_TRAIN + 1;
        end = data_size - 1;
    } else {
        init = 0;
        end = (unsigned int) ( (float) data_size ) * DATA_USED_IN_TRAIN;
    }
    
    *output = fann_subset_train_data(new_data, init, end);
    fann_destroy_train(new_data);
    new_data = NULL;

    return 0;
}
