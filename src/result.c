#include <time.h>
#include <errno.h>
#include <stdint.h>

//Adição temporaria para testes
#include <inttypes.h>
#include <stdio.h>

#include <xxhash.h>
#include <fann.h>

#include "result.h"
#include "get_params.h"

static int FANN_API callback_function(struct fann *ann, struct fann_train_data *train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs);
static int64_t time_limit_conversion(uint64_t time);

//função temporaria para testes
//static void print_results(const RESULTS * restrict const results);

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

    {
        uint64_t total_parameters = fann_get_total_connections(ann);
        if(total_parameters / 4294967296ULL != 0) { return -1; }
        results->total_parameters = (uint32_t) total_parameters;
    }
    
    results->train_time = time_limit_conversion(time);
    results->train_mse_error = fann_get_MSE(ann);
    {
        uint64_t train_bit_error = fann_get_bit_fail(ann);
        if(train_bit_error / 4294967296ULL != 0) { return -1; }
        results->train_bit_error = (uint32_t) train_bit_error;
    }


    const CONTEXT * restrict const context = (const CONTEXT *) fann_get_user_data(ann);
    if(context == NULL) {return EINVAL;}
    results->data_points_needed = context->last_epoch;
    results->version = RESULTS_COMPATIBILITY_VERSION;
    
    return 0;
}


int set_test_results(struct fann * restrict const ann, uint64_t time, RESULTS * restrict const results)
{
    if(ann == NULL) {return EINVAL;}
    if(results == NULL) {return EINVAL;}

    results->test_time = time_limit_conversion(time);
    results->test_mse_error = fann_get_MSE(ann);
    {
        uint64_t test_bit_error = fann_get_bit_fail(ann);
        if(test_bit_error / 4294967296ULL != 0) { return -1; }
        results->test_bit_error = (uint32_t) test_bit_error;
    }

    return 0;
}

int send_results(const RESULTS * restrict const results, const PARAMETERS * const restrict params)
{
    if(results == NULL) {return EINVAL;}
    if(params == NULL) {return EINVAL;}

    fwrite(&results->total_parameters, sizeof(uint32_t), 1, stdout);
    fwrite(&results->parameters_hash, sizeof(int64_t), 1, stdout);
    fwrite(&results->data_points_needed, sizeof(uint32_t), 1, stdout);
    fwrite(&results->version, sizeof(uint16_t), 1, stdout);
    fwrite(&results->train_time, sizeof(int64_t), 1, stdout);
    fwrite(&results->train_mse_error, sizeof(float), 1, stdout);
    fwrite(&results->train_bit_error, sizeof(uint32_t), 1, stdout);
    fwrite(&results->test_time, sizeof(int64_t), 1, stdout);
    fwrite(&results->test_mse_error, sizeof(float), 1, stdout);
    fwrite(&results->test_bit_error, sizeof(uint32_t), 1, stdout);
    
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

static int64_t time_limit_conversion(uint64_t time)
{
    return time / 9223372036854775808ULL != 0ULL ? -1 : (int64_t) time;
}

/*
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
*/
