#include <time.h>
#include <stdlib.h>
#include <fann.h>

#include "get_params.h"
#include "create_ann.h"
#include "data.h"
#include "train.h"
#include "result.h"

int main(int argc, char * argv[])
{
    int error_code = 0;

    struct fann * ann = NULL;
    struct fann_train_data * data = NULL;
    PARAMETERS params = { .neurons_by_layer = NULL, .activation_by_layer = NULL };
    RESULTS output;
    clock_t init = 0;
    clock_t end = 0;
    clock_t time = 0;

    error_code = get_parameter((unsigned int)argc, (const char * const *)argv, &params);
    if(error_code != 0) {goto cleanup;}

    error_code = create_configured_ann(&ann, params);
    if(error_code != 0) {goto cleanup;}

    error_code = instrument_ann(ann);
    if(error_code != 0) {goto cleanup;}

    error_code = set_parameters_hash(&params, &output);
    if(error_code != 0) {goto cleanup;}
    
    //O Setup acaba aqui

    error_code = get_train_data(&data, DATA_FILENAME);
    if(error_code != 0) {goto cleanup;}

    init = clock();

    error_code = train_network(ann, data);
    if(error_code != 0) {goto cleanup;}

    end = clock();

    time = end - init;
    init = end = 0;

    error_code = set_train_results(ann, (uint64_t) time, &output);
    if(error_code != 0) {goto cleanup;}
    fann_destroy_train(data);
    data = NULL;
    time = 0;

    
    error_code = get_test_data(&data, DATA_FILENAME);
    if(error_code != 0) {goto cleanup;}

    init = clock();

    fann_test_data(ann, data);

    end = clock();

    time = end - init;
    init = end = 0;

    error_code = set_test_results(ann, (uint64_t) time, &output);
    if(error_code != 0) {goto cleanup;}

    //Uso temporario para testes
    error_code = send_results(&output, &params, NULL);
    if(error_code != 0) {goto cleanup;}

    //Essa secao deve virar uma funcao propria
cleanup:
    if (params.neurons_by_layer) free(params.neurons_by_layer);
    if (params.activation_by_layer) free(params.activation_by_layer);
    if (ann) {
        void * context = fann_get_user_data(ann);
        if(context != NULL) { free(context); }
        fann_destroy(ann);
    }
    if (data) fann_destroy_train(data);

    return error_code;
}
