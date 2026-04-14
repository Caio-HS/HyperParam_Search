#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fann.h>

#include "get_params.h"


static int select_network_type(int input, NET_TYPE * restrict const network_type);
static int select_error_function(int input, ERROR_FUNC * restrict const error_function);
static int select_train_algorithm(int input, TRAIN_ALGO * restrict const train_algorithm);
static int select_activation_function(int input, ACTIV_FUNC * restrict const activation_function);



int get_parameter(const unsigned int argc, const char * restrict const * restrict const argv, PARAMETERS * restrict const parameters)
{
    int error_code = 0;
    /* Numero de camadas (n), ritmo de aprendizado, algoritmo de treino, metrica de erro, esparcidade, vetor de resposta
    neuronios, funcao de ativacao e stepness /p camada, */
    if(argc < MINIMUM_PARAMS) {return INCORRECT_PARAMS_COUNT;}

    //Tem que haver ao menos uma camada oculta e no maximo 128 camadas
    parameters->num_layers = (unsigned int) atoi(argv[1]);
    if(parameters->num_layers <= 0 || parameters->num_layers >= 128) {return INVALID_PARAM;}

    if(argc != MINIMUM_PARAMS + (parameters->num_layers - 1) * 2) {return INCORRECT_PARAMS_COUNT;}

    parameters->sparsity = (float) atof(argv[2]);
    if (parameters->sparsity < 0 || parameters->sparsity > 1) {return INVALID_PARAM;}

    error_code = select_network_type(atoi(argv[3]), &parameters->network_type);
    if(error_code != 0) {return error_code;}

    error_code = select_train_algorithm(atoi(argv[4]), &parameters->train_algorithm);
    if(error_code != 0) {return error_code;}

    error_code = select_error_function(atoi(argv[5]), &parameters->error_function);
    if(error_code != 0) {return error_code;}

    parameters->random_seed = (unsigned int) atoi(argv[6]);

    parameters->activation_by_layer = malloc(sizeof(ACTIV_FUNC) * parameters->num_layers);
    if(parameters->activation_by_layer == NULL) {return ENOMEM;}
    parameters->neurons_by_layer = malloc(sizeof(unsigned int) * parameters->num_layers);
    if(parameters->neurons_by_layer == NULL) 
    {
        free(parameters->activation_by_layer);
        parameters->activation_by_layer = NULL;
        return ENOMEM;
    }

    const unsigned int neurons_start_idx = MINIMUM_PARAMS - VETORIAL_PARAMS;
    const unsigned int activations_start_idx = neurons_start_idx + parameters->num_layers;

    for(unsigned int i = 0; i < parameters->num_layers; ++i) {
        parameters->neurons_by_layer[i] = (unsigned int) atoi(argv[neurons_start_idx + i]);
    }

    for(unsigned int i = 0; i < parameters->num_layers; ++i)
    {
        error_code = select_activation_function(atoi(argv[activations_start_idx + i]), &(parameters->activation_by_layer[i]));
        if(error_code != 0) {return error_code;}
    }
    return 0;
}


static int select_network_type(int input, NET_TYPE * restrict const network_type)
{
    switch(input)
    {
        case 0:
            *network_type = FANN_NETTYPE_LAYER;
            return 0;
        case 1:
            *network_type = FANN_NETTYPE_SHORTCUT;
            return 0;
        default:
            return INVALID_PARAM;
    }
}


static int select_error_function(int input, ERROR_FUNC * restrict const error_function)
{
    switch(input)
    {
        case 0:
            *error_function = FANN_ERRORFUNC_LINEAR;
            return 0;
        case 1:
            *error_function = FANN_ERRORFUNC_TANH;
            return 0;
        default:
            return INVALID_PARAM;
    }
}


static int select_train_algorithm(int input, TRAIN_ALGO * restrict const train_algorithm)
{
    switch(input)
    {
        case 0:
            *train_algorithm = FANN_TRAIN_INCREMENTAL;
            return 0;
        case 1:
            *train_algorithm = FANN_TRAIN_BATCH;
            return 0;
        case 2:
            *train_algorithm = FANN_TRAIN_RPROP;
            return 0;
        case 3:
            *train_algorithm = FANN_TRAIN_QUICKPROP;
            return 0;
        case 4:
            *train_algorithm = FANN_TRAIN_SARPROP;
            return 0;
        default:
            return INVALID_PARAM;
    }
}


static int select_activation_function(int input, ACTIV_FUNC * restrict const activation_function)
{
    switch(input)
    {
        case 0:
            *activation_function = FANN_LINEAR;
            return 0;
        case 1:
            *activation_function = FANN_THRESHOLD;
            return 0;
        case 2:
            *activation_function = FANN_THRESHOLD_SYMMETRIC;
            return 0;
        case 3:
            *activation_function = FANN_SIGMOID;
            return 0;
        case 4:
            *activation_function = FANN_SIGMOID_STEPWISE;
            return 0;
        case 5:
            *activation_function = FANN_SIGMOID_SYMMETRIC;
            return 0;
        case 6:
            *activation_function = FANN_SIGMOID_SYMMETRIC_STEPWISE;
            return 0;
        case 7:
            *activation_function = FANN_GAUSSIAN;
            return 0;
        case 8:
            *activation_function = FANN_GAUSSIAN_SYMMETRIC;
            return 0;
        case 9:
            *activation_function = FANN_GAUSSIAN_STEPWISE;
            return 0;
        case 10:
            *activation_function = FANN_ELLIOT;
            return 0;
        case 11:
            *activation_function = FANN_ELLIOT_SYMMETRIC;
            return 0;
        case 12:
            *activation_function = FANN_LINEAR_PIECE;
            return 0;
        case 13:
            *activation_function = FANN_LINEAR_PIECE_SYMMETRIC;
            return 0;
        case 14:
            *activation_function = FANN_SIN_SYMMETRIC;
            return 0;
        case 15:
            *activation_function = FANN_COS_SYMMETRIC;
            return 0;
        case 16:
            *activation_function = FANN_SIN;
            return 0;
        case 17:
            *activation_function = FANN_COS;
            return 0;
        default:
            return INVALID_PARAM;
    }
}
