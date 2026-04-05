#include <fann.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#include "result.h"

static int FANN_API callback_function(struct fann *ann, struct fann_train_data *train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs);

int instrument_ann(struct fann * restrict const ann, CONTEXT ** restrict const context)
{
    if(ann == NULL) {return EINVAL;}

    *context = (CONTEXT *) malloc(sizeof(CONTEXT));
    if(*context == NULL) {return ENOMEM;}

    (*context)->last_epoch = 0;
    
    fann_set_user_data(ann, *context);

    fann_set_callback(ann, callback_function);

    return 0;
}

int set_train_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}


    results->total_parameters = (uint64_t) fann_get_total_connections(ann);
    results->train_time = time;
    results->train_mse_error = fann_get_MSE(ann);
    results->train_bit_error = (uint64_t) fann_get_bit_fail(ann);

    const CONTEXT * restrict const context = (const CONTEXT *) fann_get_user_data(ann);
    results->data_points_needed = context->last_epoch;
    return 0;
}


int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}

    results->test_time = time;
    results->test_mse_error = fann_get_MSE(ann);
    results->test_bit_error = (uint64_t) fann_get_bit_fail(ann);

    return 0;
}

int send_results(RESULTS * restrict const results, RESULTS * restrict const output)
{
    if(results == NULL) {return EINVAL;}
    if(output == NULL) {return EINVAL;}

    
    return 0;
}


static int FANN_API callback_function(struct fann *ann, struct fann_train_data *train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs)
{
    if (ann == NULL) {return -1;}
    if (train == NULL) {return -1;}
    if (max_epochs < epochs) {return -1;}
    if (unsigned_between_reports == 0) {return -1;}
    if (desired_error == 0) {return -1;}
    
    CONTEXT * const restrict context = fann_get_user_data(ann);
    context->last_epoch += 1;
    return 0;
}


