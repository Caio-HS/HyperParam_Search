import sys

# Mapeamento estrito dos Enums do FANN/get_params.c
NET_TYPE = {0: "FANN_NETTYPE_LAYER", 1: "FANN_NETTYPE_SHORTCUT"}
ERROR_FUNC = {0: "FANN_ERRORFUNC_LINEAR", 1: "FANN_ERRORFUNC_TANH"}
TRAIN_ALGO = {
    0: "FANN_TRAIN_INCREMENTAL",
    1: "FANN_TRAIN_BATCH",
    2: "FANN_TRAIN_RPROP",
    3: "FANN_TRAIN_QUICKPROP",
    4: "FANN_TRAIN_SARPROP"
}
ACTIV_FUNC = {
    0: "FANN_LINEAR", 1: "FANN_THRESHOLD", 2: "FANN_THRESHOLD_SYMMETRIC",
    3: "FANN_SIGMOID", 4: "FANN_SIGMOID_STEPWISE", 5: "FANN_SIGMOID_SYMMETRIC",
    6: "FANN_SIGMOID_SYMMETRIC_STEPWISE", 7: "FANN_GAUSSIAN", 8: "FANN_GAUSSIAN_SYMMETRIC",
    9: "FANN_GAUSSIAN_STEPWISE", 10: "FANN_ELLIOT", 11: "FANN_ELLIOT_SYMMETRIC",
    12: "FANN_LINEAR_PIECE", 13: "FANN_LINEAR_PIECE_SYMMETRIC", 14: "FANN_SIN_SYMMETRIC",
    15: "FANN_COS_SYMMETRIC", 16: "FANN_SIN", 17: "FANN_COS"
}

class NetworkParameters:
    def __init__(self):
        self.num_layers = 0
        self.sparsity = 0.0
        self.network_type = 0
        self.train_algorithm = 0
        self.error_function = 0
        self.random_seed = 0
        self.neurons_by_layer = []
        self.activation_by_layer = []

def parse_parameters_string(param_string: str) -> NetworkParameters:
    # 1. Transforma a string contínua em um array no estilo "argv"
    # O primeiro elemento simula o nome do executável para bater com os índices do C
    argv = ["worker"] + param_string.strip().split()
    argc = len(argv)
    
    MINIMUM_PARAMS = 9
    VETORIAL_PARAMS = 2

    if argc < MINIMUM_PARAMS:
        raise ValueError("INCORRECT_PARAMS_COUNT")

    parameters = NetworkParameters()

    # Validando número de camadas (Índice 1)
    parameters.num_layers = int(argv[1])
    if parameters.num_layers <= 0 or parameters.num_layers >= 128:
        raise ValueError("INVALID_PARAM: num_layers")

    # Validando contagem exata de argumentos esperados
    if argc != MINIMUM_PARAMS + (parameters.num_layers - 1) * 2:
        raise ValueError("INCORRECT_PARAMS_COUNT")

    # Sparsity (Índice 2)
    parameters.sparsity = float(argv[2])
    if parameters.sparsity < 0.0 or parameters.sparsity > 1.0:
        raise ValueError("INVALID_PARAM: sparsity")

    # Network Type (Índice 3)
    net_type_val = int(argv[3])
    if net_type_val not in NET_TYPE:
        raise ValueError("INVALID_PARAM: network_type")
    parameters.network_type = net_type_val

    # Train Algorithm (Índice 4)
    train_algo_val = int(argv[4])
    if train_algo_val not in TRAIN_ALGO:
        raise ValueError("INVALID_PARAM: train_algorithm")
    parameters.train_algorithm = train_algo_val

    # Error Function (Índice 5)
    error_func_val = int(argv[5])
    if error_func_val not in ERROR_FUNC:
        raise ValueError("INVALID_PARAM: error_function")
    parameters.error_function = error_func_val

    # Random Seed (Índice 6)
    parameters.random_seed = int(argv[6])

    # Lógica de fatiamento idêntica à do C para os vetores
    neurons_start_idx = MINIMUM_PARAMS - VETORIAL_PARAMS
    activations_start_idx = neurons_start_idx + parameters.num_layers

    # Populando neurônios por camada
    for i in range(parameters.num_layers):
        parameters.neurons_by_layer.append(int(argv[neurons_start_idx + i]))

    # Populando funções de ativação por camada com validação
    for i in range(parameters.num_layers):
        act_val = int(argv[activations_start_idx + i])
        if act_val not in ACTIV_FUNC:
            raise ValueError(f"INVALID_PARAM: activation_by_layer em index {i}")
        parameters.activation_by_layer.append(act_val)

    return parameters
