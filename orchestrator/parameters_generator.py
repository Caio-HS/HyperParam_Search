import random
import parameters as params

current_network_type = 0
current_train_algorithm = 0
current_error_function = 0
current_density = 0
current_num_layers = 0

def next_param():
  for num_layers in params.num_layers:
    for density in params.density:
      for network_type in params.network_type:
        for train_algorithm in params.train_algorithm:
          for error_function in params.error_function:
            config = str(num_layers) + " " + str(density) + " " + str(network_type) + " " + str(train_algorithm) + " " + str(error_function) + " " + str(random.randint(0, 2**64 - 1))
    for i in range(num_layers):

            
