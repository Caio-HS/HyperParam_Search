#include <fann.h>
#include <time.h>
#include <errno.h>

#include "result.h"

int instrument_ann(struct fann * restrict const ann, CONTEXT ** restrict const context)
{
    if(ann == NULL) {return EINVAL;}

    *context = malloc(sizeof(CONTEXT));
    if(*context == NULL) {return ENOMEM;}

    fann_set_user_data(ann, *context);

    return 0;
}

int set_train_results(struct fann * restrict const ann, uint64_t train_time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}


    results->total_parameters = fann_get_total_connections(ann);
    results->train_time = train_time;
    results->train_mse_error = fann_get_MSE(ann);
    results->train_bit_error = fann_get_bit_fail(ann);

    const CONTEXT * restrict const context = (const CONTEXT *) fann_get_user_data(ann);
    results->data_points_needed = context->last_epochs;
    return 0;
}


int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}

    results->test_time = train_time;
    results->test_mse_error = fann_get_MSE(ann);
    results->test_bit_error = fann_get_bit_fail(ann);

    return 0;
}

int send_results(RESULTS * restrict const results, int * restrict const result_vector)
{
    if(results == NULL) {return EINVAL;}
    if(result_vector == NULL) {return EINVAL;}
    return 0;
}
