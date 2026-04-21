num_layers = [ 4, 6, 10 ] # Potencias de 2 de 2^1 a 2^3  + 2, Camada de entrada + 2^n camadas ocultas + Camada de saida
density = [ 0.01, 0.10, 0.50, 0.90, 1.0 ] # Quantos % das conexoes entre neuronios vao existir, 1.0 significa 100%, 0.01 significa 1%
network_type = [ 0, 1 ] 
error_function = [ 0, 1 ] 
train_algorithm = [ 0, 1, 2 ]
activation_function = [ 0, 1, 3, 7, 14 ]
neuron_numbers = [ 2, 4, 8, 16, 32 ]
