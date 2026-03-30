#include <fann.h>
#include <time.h>
#include <errno.h>

#include "results.h"

int instrument_ann(struct fann * restrict const ann)
{
    if(ann == NULL) {return EINVAL;}
    return 0;
}

int set_results(struct fann * restrict const ann, uint64_t train_time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}
    return 0;
}

int send_results(RESULTS * restrict const results, int * restrict const result_vector)
{
    if(results == NULL) {return EINVAL;}
    if(result_vector == NULL) {return EINVAL;}
    return 0;
}
