#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <fann.h>

#include "get_params.h"
#include "create_ann.h"


static int check_parameters(const PARAMETERS params);
static int create_sparse_ann(struct fann ** restrict const ann, const PARAMETERS params);
static int create_shortcut_ann(struct fann ** restrict const ann, const PARAMETERS params);
static int create_common_ann(struct fann ** restrict  const ann, const PARAMETERS params);
static int configure_ann(struct fann * restrict const ann, const PARAMETERS params);
static int set_activation_function(struct fann * const ann, const PARAMETERS params);



int create_configured_ann(struct fann ** restrict const ann, const PARAMETERS params)
{
    if(ann == NULL) { return EINVAL;}

    int error_code = 0;
    error_code = check_parameters(params);
    if(error_code != 0) {return error_code;}

    if(params.sparsity < SPARSE_THERESHOLD)
    {
        error_code = create_sparse_ann(ann, params);
        if(error_code != 0) {return error_code;}
    } else if (params.network_type == FANN_NETTYPE_SHORTCUT) {
        error_code = create_shortcut_ann(ann, params);
        if(error_code != 0) {return error_code;}
    } else {
        error_code = create_common_ann(ann, params);
        if(error_code != 0) {return error_code;}
    }

    error_code = configure_ann(*ann, params);
    if(error_code != 0) {return error_code;}

    return 0;

}


static int check_parameters(const PARAMETERS params)
{
    if(params.num_layers == 0) {return EINVAL;}
    if(params.neurons_by_layer == NULL) {return EINVAL;}
    if(params.activation_by_layer == NULL) {return EINVAL;}
    return 0;
}


static int create_sparse_ann(struct fann ** restrict const ann, const PARAMETERS params)
{
    *ann = fann_create_sparse_array(params.sparsity, params.num_layers, params.neurons_by_layer);
    if(*ann == NULL) {return ENOMEM;}
    return 0;
}


static int create_shortcut_ann(struct fann ** restrict const ann, const PARAMETERS params)
{
    *ann = fann_create_shortcut_array(params.num_layers, params.neurons_by_layer);
    if(*ann == NULL) {return ENOMEM;}
    return 0;
}


static int create_common_ann(struct fann ** restrict const ann, const PARAMETERS params)
{
    *ann = fann_create_standard_array(params.num_layers, params.neurons_by_layer);
    if(*ann == NULL) {return ENOMEM;}
    return 0;
}


static int configure_ann(struct fann * restrict const ann, const PARAMETERS params)
{
    if(ann == NULL){return EINVAL;}

    int error_code = 0;

    srand(params->random_seed);
    
    fann_randomize_weights(ann, MIN_WEIGHT, MAX_WEIGHT);

    fann_set_training_algorithm(ann, params.train_algorithm);

    fann_set_train_error_function(ann, params.error_function);

    error_code = set_activation_function(ann, params);
    if(error_code != 0) {return error_code;}

    return 0;
}


static int set_activation_function(struct fann * const ann, const PARAMETERS params)
{
    for(unsigned int layer = 1; layer < params.num_layers; ++layer)
    {
        fann_set_activation_function_layer(ann, *(params.activation_by_layer + layer), (int) layer);
    }
    return 0;
}
