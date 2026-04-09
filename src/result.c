#include <fann.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

//Adição temporaria para testes
#include <inttypes.h>
#include <stdio.h>

#include "result.h"
#include "get_params.h"

static int FANN_API callback_function(struct fann *ann, struct fann_train_data *train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs);

//função temporaria para testes
static void print_results(const RESULTS * restrict const results);

int instrument_ann(struct fann * restrict const ann)
{
    if(ann == NULL) {return EINVAL;}

    CONTEXT * restrict context = (CONTEXT *) malloc(sizeof(CONTEXT));
    if(context == NULL) {return ENOMEM;}

    context->last_epoch = 0;
    
    fann_set_user_data(ann, context);

    fann_set_callback(ann, callback_function);

    return 0;
}

int set_train_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}

    results->total_parameters = (uint64_t) fann_get_total_connections(ann);
    results->train_time = time;
    results->train_mse_error = fann_get_MSE(ann);
    results->train_bit_error = (uint64_t) fann_get_bit_fail(ann);

    const CONTEXT * restrict const context = (const CONTEXT *) fann_get_user_data(ann);
    if(context == NULL) {return EINVAL;}
    results->data_points_needed = context->last_epoch;
    return 0;
}


int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}

    results->test_time = time;
    results->test_mse_error = fann_get_MSE(ann);
    results->test_bit_error = (uint64_t) fann_get_bit_fail(ann);

    return 0;
}

int send_results(const RESULTS * restrict const results, const PARAMETERS * const restrict params, RESULTS * restrict const output)
{
    if(results == NULL) {return EINVAL;}
    /* Modificação temporaria para testes
    if(output == NULL) {return EINVAL;}
    */
    if(params == NULL) {return EINVAL;}

    //função temporaria para testes
    print_results(results);
    
    return 0;
}


static int FANN_API callback_function(struct fann *ann, struct fann_train_data *train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs)
{
    if (ann == NULL) {return -1;}
    if (train == NULL) {return -1;}
    if (max_epochs < epochs) {return -1;}
    if (epochs_between_reports == 0) {return -1;}
    if (desired_error < 0) {return -1;}

    CONTEXT * const restrict context = fann_get_user_data(ann);
    if(context == NULL) 
    {
        fprintf(stderr, "Erro: ANN without context structure");
        return -1;
    }
    
    context->last_epoch += 1;
    return 0;
}


//função temporaria para testes
void print_results(const RESULTS * restrict const results) 
{
    if (results == NULL) {
        fprintf(stderr, "Erro: Ponteiro de resultados nulo.\n");
        return;
    }

    printf("\n========================================\n");
    printf("        RELATÓRIO DE EXECUÇÃO\n");
    printf("========================================\n");

    // Informações da Rede
    printf("Total de Parâmetros: %" PRIu64 "\n", results->total_parameters);
    printf("Épocas necessárias:  %" PRIu64 "\n", results->data_points_needed);

    // Métricas de Treino
    printf("\n--- Treinamento ---\n");
    printf("Tempo de Treino:     %" PRIu64 " clocks\n", results->train_time);
    printf("Erro MSE (Treino):   %f\n", (double) results->train_mse_error);
    printf("Erro de Bits:        %" PRIu64 "\n", results->train_bit_error);

    // Métricas de Teste
    printf("\n--- Teste ---\n");
    printf("Tempo de Teste:      %" PRIu64 " clocks\n", results->test_time);
    printf("Erro MSE (Teste):    %f\n", (double) results->test_mse_error);
    printf("Erro de Bits:        %" PRIu64 "\n", results->test_bit_error);
    
    printf("========================================\n\n");
}
