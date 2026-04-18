#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <xxhash.h>

#include "get_params.h"
#include "result.h"

static int get_parameters_vector (const PARAMETERS * restrict const params, void * restrict * restrict const vector_output, unsigned int * restrict const size);
static int alloc_vector (unsigned int num_layers, void * restrict * restrict const vector_output, unsigned int * restrict const size);
static int write_parameters_in_vector (const PARAMETERS * restrict const params, void * restrict * restrict const target_vector, const unsigned int size);

int set_parameters_hash(const PARAMETERS * restrict const params, RESULTS * restrict const results)
{
    if(params == NULL) { return EINVAL; }
    if(results == NULL) { return EINVAL; }

    int error_code = 0;

    void * restrict parameters_vector = NULL;
    unsigned int size = 0;

    error_code = get_parameters_vector(params, &parameters_vector, &size);
    if(error_code != 0) { return error_code; }

    results->parameters_hash = XXH64(parameters_vector, (size_t) size, HASHS_SEED);

    free(parameters_vector);
    
    return 0;
}

static int get_parameters_vector (const PARAMETERS * restrict const params, void * restrict * restrict const vector_output, unsigned int * restrict const size)
{
    if(params == NULL) { return EINVAL; }
    if(vector_output == NULL) { return EINVAL; }
    if(size == NULL) { return EINVAL; }

    int error_code = 0;

    error_code = alloc_vector(params->num_layers, vector_output, size);
    if(error_code != 0) { return error_code; }

    error_code = write_parameters_in_vector(params, vector_output, (*size) );
    if(error_code != 0) { return error_code; }

    return 0;
}

static int alloc_vector (unsigned int num_layers, void * restrict * restrict const vector_output, unsigned int * restrict const size)
{
    if(vector_output == NULL) { return EINVAL; }
    if(size == NULL) { return EINVAL; }

    (*size) = (unsigned int) (sizeof(unsigned int) + sizeof(float) + sizeof(NET_TYPE) + sizeof(TRAIN_ALGO) + sizeof(ERROR_FUNC) + sizeof(unsigned int) + num_layers * ( (unsigned int) (sizeof(unsigned int) + sizeof(ACTIV_FUNC)) ));

    (*vector_output) = malloc( (*size) );
    if( (*vector_output) ) { return ENOMEM; }

    return 0;
}

static int write_parameters_in_vector (const PARAMETERS * restrict const params, void * restrict * restrict const target_vector, const unsigned int size)
{
    if(params == NULL) { return EINVAL; }
    if(target_vector == NULL) { return EINVAL; }
    if(size == 0) { return EINVAL; }

    size_t actual_shift = 0;
    size_t data_size = 0;

    data_size = sizeof(unsigned int);
    memcpy((*target_vector + actual_shift), &params->num_layers, data_size);
    actual_shift += data_size;

    data_size = sizeof(float);
    memcpy((*target_vector + actual_shift), &params->sparsity, data_size);
    actual_shift += data_size;

    data_size = sizeof(NET_TYPE);
    memcpy((*target_vector + actual_shift), &params->network_type, data_size);
    actual_shift += data_size;

    data_size = sizeof(TRAIN_ALGO);
    memcpy((*target_vector + actual_shift), &params->train_algorithm, data_size);
    actual_shift += data_size;

    data_size = sizeof(ERROR_FUNC);
    memcpy((*target_vector + actual_shift), &params->error_function, data_size);
    actual_shift += data_size;

    data_size = sizeof(unsigned int);
    memcpy((*target_vector + actual_shift), &params->random_seed, data_size);
    actual_shift += data_size;

    for (int i = 0; i < params->num_layers; i++)
    {
        data_size = sizeof(unsigned int);
        memcpy((*target_vector + actual_shift), &(params->neurons_by_layer[i]), data_size);
        actual_shift += data_size;
    }

    for (int i = 0; i < params->num_layers; i++)
    {
        data_size = sizeof(ACTIV_FUNC);
        memcpy((*target_vector + actual_shift), &(params->activation_by_layer[i]), data_size);
        actual_shift += data_size;
    }

    return 0;
}
