#include <fann.h>
#include <time.h>
#include <errno.h>

#include "results.h"

int instrument_ann(struct fann * restrict const ann, CONTEXT ** restrict const context)
{
    if(ann == NULL) {return EINVAL;}

    *context = malloc(sizeof(CONTEXT));
    if(*context == NULL) {return ENOMEM;}

    fann_set_user_data(ann, *context);
    
    return 0;
}

int set_results(struct fann * restrict const ann, const CONTEXT * restrict const context, uint64_t train_time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}
    if(context == NULL {return EINVAL;}
    return 0;
}

int send_results(RESULTS * restrict const results, int * restrict const result_vector)
{
    if(results == NULL) {return EINVAL;}
    if(result_vector == NULL) {return EINVAL;}
    return 0;
}
