#ifndef GET_PARAMS_H
#define GET_PARAMS_H

#include <stdbool.h>
#include <fann.h>

#define INCORRECT_PARAMS_COUNT 1u
#define INVALID_PARAM 2u
#define VETORIAL_PARAMS 2u
#define MINIMUM_PARAMS 8

typedef enum fann_errorfunc_enum ERROR_FUNC;
typedef enum fann_train_enum TRAIN_ALGO;
typedef enum fann_activationfunc_enum ACTIV_FUNC;
typedef enum fann_nettype_enum NET_TYPE;


struct network_parameters
{
	unsigned int num_layers;
    float sparsity;
    NET_TYPE network_type;
    TRAIN_ALGO train_algorithm;
    ERROR_FUNC error_function;

	unsigned int * restrict neurons_by_layer;
    ACTIV_FUNC * restrict activation_by_layer;
	//float * restrict steepness_by_layer;
    //float learn_rate;
};

typedef struct network_parameters PARAMETERS;

int get_parameter(const unsigned int argc, const char * restrict const * restrict const argv, PARAMETERS * restrict const parameters);

#endif
