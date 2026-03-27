#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fann.h>

#include "get_params.h"

static int select_network_type(int input, NET_TYPE * restrict const network_type);
static int select_error_function(int input, ERROR_FUNC * restrict const error_function);
static int select_train_algorithm(int input, TRAIN_ALGO * restrict const train_algorithm);
static int select_activation_function(int input, ACTIV_FUNC * restrict const activation_function);

int get_parameter(const unsigned int argc, const char * restrict const argv[], PARAMETERS * restrict const parameters)
{

    int error_code = 0;
    /* Numero de camadas (n), ritmo de aprendizado, algoritmo de treino, metrica de erro, esparcidade, vetor de resposta 
    neuronios, funcao de ativacao e stepness /p camada, */
    if(argc < MINIMUM_PARAMS) {return INCORRECT_PARAMS_COUNT;}

    //Tem que haver ao menos uma camada oculta e no maximo 128 camadas
    int parameters->num_layers = atoi(argv[1]);
    if(parameters->num_layers <= 0 || parameters->num_layers >= 128) {return INVALID_PARAM;}

    if(argc != MINUMUM_PARAMS + (parameters->num_layers - 1) * 2) {return INCORRECT_PARAMS_COUNT;}

    parameters->sparcity = atof(argv[2]);
    if (parameters->sparcity < 0 || parameters->sparcity > 1) {return INVALID_PARAM;}

    error_code = select_network_type(atoi(argv[3]), &parameters->network_type);
    if(error_code != 0) {return error_code;}

    error_code = select_train_algorithm(atoi(argv[4]), &parameters->train_algorithm);
    if(error_code != 0) {return error_code;}

    error_code = select_error_function(atoi(argv[5]), &parameters->error_function);
    if(error_code != 0) {return error_code;}

    parameters->activation_by_layer = malloc(sizeof(ACTIV_FUNC) * parameters->num_layers);
    if(parameters->activation_by_layer == NULL) {return EOMEM;}
    parameters->neuron_by_layer = malloc(sizeof(unsigned int) * parameters->num_layers);
    if(parameters->neuron_by_layer == NULL) {return EOMEM;}

    for(int i = 0; i < num_layers; i++) {parameters->neuron_by_layer[i] = atoi(argv[(MINIMUM_PARAMS - 2) + parameters->num_layers * 0 + i]);}
    for(int i = 0; i < num_layers; i++)
    {
        error_code = select_error_function(atoi(argv[(MINIMUM_PARAMS - 2) + parameters->num_layers * 1 + i]), &(parameters->activation_by_layer[i]));
        if(error_code != 0) {return error_code;}
    }
    return 0;
}
