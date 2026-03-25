#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fann.h>

#include "get_params.h"

int get_parameter(const unsigned int argc, const char * restrict const argv[], PARAMETERS * restrict const parameters)
{
    /* Numero de camadas (n), ritmo de aprendizado, algoritmo de treino, metrica de erro, esparcidade, vetor de resposta 
    neuronios, funcao de ativacao e stepness /p camada, */
    if(argc < MINIMUM_PARAMS) {return INCORRECT_PARAMS_COUNT;}

    //Tem que haver ao menos uma camada oculta e no maximo 128 camadas
    int (*parameters).num_layers = atoi(argv[1]);
    if((*parameters).num_layers <= 0 || (*parameters).num_layers >= 128) {return INVALID_PARAM;}

    if(argc != MINUMUM_PARAMS + ((*parameters).num_layers - 1) * 2) {return INCORRECT_PARAMS_COUNT;}

    

    int train_algo = atoi(argv[3]);

    int error_metric = atoi(argv[4]);

    double esparcidade = atoi(argv[5]);

    int* response_vector = (int*) atoi(argv[6]);

    int neurons_by_layer[num_layers];
    int layers_function[ num_layers];
    int layers_stepness[ num_layers];

    for(int i = 0; i < num_layers; i++) {neurons_by_layer[5 + i + 0 * num_layers];}
    for(int i = 0; i < num_layers; i++) {layers_function[  5 + i + 1 * num_layers];}
    for(int i = 0; i < num_layers; i++) {layers_stepness[  5 + i + 2 * num_layers];}



    //Criacao estrutura da rede
    struct fann *ann = NULL;



    //Medida do tempo
    int treino_inicio = 0;



    return 0;
}
