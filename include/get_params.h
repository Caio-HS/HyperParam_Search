#ifndef GET_PARAMS_H
#define GET_PARAMS_H

#include <stdbool.h>
#include <fann.h>

#define INCORRECT_PARAMS_COUNT 0b1
#define INVALID_PARAM 0b10
#define MINIMUM_PARAMS 7

typedef enum error_function_enum ERROR_FUNC;
typedef enum training_algorithm_enum TRAIN_ALGO;
typedef enum activation_function_enum ACTIV_FUNC;
typedef enum network_type_enum NET_TYPE;


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

int get_parameter(const unsigned int argc, const char * restrict const argv[], PARAMETERS * restrict const parameters);

#endif
