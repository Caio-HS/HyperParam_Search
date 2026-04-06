#include <time.h>
#include <stdlib.h>

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

    error_code = get_parameter((unsigned int)argc, (const char * const *)argv, &params);
    if(error_code != 0) {goto cleanup;}

    error_code = create_configured_ann(&ann, params);
    if(error_code != 0) {goto cleanup;}

    error_code = instrument_ann(ann);
    if(error_code != 0) {goto cleanup;}
    
    //O Setup acaba aqui
    clock_t init = 0;
    clock_t end = 0;
    clock_t time = 0;

    error_code = get_train_data(&data, DATA_FILENAME);
    if(error_code != 0) {goto cleanup;}

    init = clock();

    error_code = train_network(ann, data, &output);
    if(error_code != 0) {goto cleanup;}

    end = clock();

    time = end - init;    

    error_code = set_train_results(struct fann * restrict const ann, time, &results);
    if(error_code != 0) {goto cleanup;}
    fann_destroy_train(data);
    data = NULL;

    
    error_code = get_train_data(&data, DATA_FILENAME);
    if(error_code != 0) {goto cleanup;}

    init = clock();

    error_code = fann_test_data(ann, data);
    if(error_code != 0) {goto cleanup;}

    end = clock();

    time = end - init;    

    error_code = set_test_results(struct fann * restrict const ann, time, &output);
    if(error_code != 0) {goto cleanup;}

    //Essa secao deve virar uma funcao propria
cleanup:
    if (params.neurons_by_layer) free(params.neurons_by_layer);
    if (params.activation_by_layer) free(params.activation_by_layer);
    if (ann) fann_destroy(ann);
    if (data) fann_destroy_train(data);

    return error_code;
}
