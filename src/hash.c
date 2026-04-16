#include <stdlib.h>

#include <xxhash.h>

#include "get_params.h"
#include "result.h"

static int get_parameters_vector (const PARAMETERS * restrict const params, const void * restrict * restrict const vector_output, unsigned int * restrict const size);

int set_parameters_hash(const PARAMETERS * restrict const params, RESULTS * restrict const results)
{
    if(params == NULL) { return EINVAL; }
    if(results == NULL) { return EINVAL; }

    int error_code = 0;

    const void * restrict parameters_vector = NULL;
    unsigned int size = 0;

    error_code = get_parameters_vector(params, &parameters_vector, &size);
    if(error_code != 0) { return error_code; }

    results->parameters_hash = XXH64(parameters_vector, (size_t) size, HASHS_SEED);
    
    return 0;
}

static int get_parameters_vector (const PARAMETERS * restrict const params, const void * restrict * restrict const vector_output, unsigned int * restrict const size)
{
    if(params == NULL) { return EINVAL; }
    if(vector_output == NULL) { return EINVAL; }
    if(size == NULL) { return EINVAL; }

    return 0;
}
