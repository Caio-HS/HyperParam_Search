#include "get_params.h"
#include "create_ann.h"

int main(int argc, char * argv[])
{
    int error_code = 0;

    struct fann * restrict ann = NULL;
    PARAMETERS params;

    error_code = get_parameter(argc, argv, &params);
    if(error_code != 0) {return error_code;}
    
    error_code = create_configured_ann(&ann, params); 
    if(error_code != 0) {return error_code;}

    //Essa secao deve virar uma funcao propria
    free(params.neurons_by_layer);
    free(params.activation_by_layer);
    fann_destroy(ann);

    return 0;
}
