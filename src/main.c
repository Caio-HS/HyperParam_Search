#include <stdlib.h>

#include "get_params.h"
#include "create_ann.h"

int main(int argc, char * argv[])
{
    int error_code = 0;

    struct fann * ann = NULL;
    PARAMETERS params = { .neurons_by_layer = NULL, .activation_by_layer = NULL };

    error_code = get_parameter((unsigned int)argc, (const char * const *)argv, &params);
    if(error_code != 0) {goto cleanup;}
    
    error_code = create_configured_ann(&ann, params); 
    if(error_code != 0) {goto cleanup;}

    //Essa secao deve virar uma funcao propria
cleanup:
    if (params.neurons_by_layer) free(params.neurons_by_layer);
    if (params.activation_by_layer) free(params.activation_by_layer);
    if (ann) fann_destroy(ann);

    return error_code;
}
