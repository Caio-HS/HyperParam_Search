import random
import parameters as params

parameters_genereted = 0
layer_index = 0

def update_layer_index(count, layers):
  num_layers = params.num_layers[layer_index]
  density_options = len(params.density)
  network_type_options = len(params.network_type)
  error_function_options = len(params.error_function)
  train_algorithm_options = len(params.train_algorithm)
  activation_function_options = len(params.activation_function)
  neuron_numbers_options = len(params.neuron_numbers)
  
  total_combinations = density_options * network_type_options * error_function_options * train_algorithm_options * neuron_numbers_options**num_layers * activation_function_options**num_layers
  
  if (count + 1) % total_combinations == 0:
    return -1, layers + 1
    
  return count, layers

def save_state(count, layers):
  global parameters_genereted
  parameters_genereted = count + 1
  global layer_index
  layer_index = layers

def mount_command(current_num_layers, current_density, current_network_type, current_train_algorithm, current_error_function, current_neuron_numbers, current_activation_function):
  cmd_parts = [
      str(current_num_layers),
      str(current_density),
      str(current_network_type),
      str(current_train_algorithm),
      str(current_error_function),
      str(random.randint(0, 2**32 - 1))
  ]
    

  cmd_parts.extend(str(n) for n in current_neuron_numbers)
  cmd_parts.extend(str(a) for a in current_activation_function)
    
  return " ".join(cmd_parts)  

def pic_value(array, counter, previous_combinations):
  position = (counter // (previous_combinations)) % len(array)
  return array[position], previous_combinations * len(array)

def next_param():
  if layer_index >= len(params.num_layers):
    return None
  previous_combinations = 1
  
  current_density, previous_combinations = pic_value(params.density, parameters_genereted, previous_combinations)
  current_network_type, previous_combinations = pic_value(params.network_type, parameters_genereted, previous_combinations)
  current_error_function, previous_combinations = pic_value(params.error_function, parameters_genereted, previous_combinations)
  current_train_algorithm, previous_combinations = pic_value(params.train_algorithm, parameters_genereted, previous_combinations)
  
  current_num_layers = params.num_layers[layer_index]
  
  current_activation_function = [-1 for _ in range(current_num_layers)]
  for i in range(current_num_layers):
    current_activation_function[i], previous_combinations = pic_value(params.activation_function, parameters_genereted, previous_combinations)
    
  current_neuron_numbers = [-1 for _ in range(current_num_layers)]
  for i in range(current_num_layers):
    current_neuron_numbers[i], previous_combinations = pic_value(params.neuron_numbers, parameters_genereted, previous_combinations)

  #GAMBIARRA TEMPORARIA PARA EVITAR MISMATCH ENTRE DATASET E PARAMETROS
  current_neuron_numbers[0] = 8
  current_neuron_numbers[-1] = 2
    
  next_parameters_genereted = parameters_genereted
  next_layer_index = layer_index
  
  next_parameters_genereted, next_layer_index = update_layer_index(parameters_genereted, layer_index)

  save_state(next_parameters_genereted, next_layer_index)
  
  return mount_command(current_num_layers, current_density, current_network_type, current_train_algorithm, current_error_function, current_neuron_numbers, current_activation_function)
  
