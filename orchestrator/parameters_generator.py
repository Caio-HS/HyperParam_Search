import random
import parameters as params

parameters_genereted = 0
layers_used = 2

def save_state(count, layers):
  global parameters_genereted
  parameters_genereted = count + 1
  global layers_used
  layers_used = layers

def mount_command(current_num_layers, current_density, current_network_type, current_train_algorithm, current_error_function, current_neuron_numbers, current_activation_function):
  parameters_command = ""
  parameters_command += str(current_num_layers)
  parameters_command += " " + str(current_density)
  parameters_command += " " + str(current_network_type)
  parameters_command += " " + str(current_train_algorithm)
  parameters_command += " " + str(current_error_function)
  parameters_command += " " + str(random.randint(0, 2**64 - 1))
    
  for i in range(int(current_num_layers)):
    parameters_command += " " + str(current_neuron_numbers[i])

  for i in range(int(current_num_layers)):
    parameters_command += " " + str(current_activation_function[i])
        
  return parameters_command
  

def pic_value(array, counter, previous_combinations):
  position = (counter // (previous_combinations)) % len(array)
  return array[position], previous_combinations * len(array)

def next_param():
  previous_combinations = 1
  
  current_density, previous_combinations = pic_value(params.density, parameters_genereted, previous_combinations)
  current_network_type, previous_combinations = pic_value(params.network_type, parameters_genereted, previous_combinations)
  current_error_function, previous_combinations = pic_value(params.error_function, parameters_genereted, previous_combinations)
  current_train_algorithm, previous_combinations = pic_value(params.train_algorithm, parameters_genereted, previous_combinations)

  current_activation_function = [-1 for _ in range(layers_used)]
  for i in range(layers_used):
    current_activation_function[i], previous_combinations = pic_value(params.activation_function, parameters_genereted, previous_combinations)
    
  current_num_layers = layers_used
  next_layers_used, previous_combinations = pic_value(params.num_layers, parameters_genereted, previous_combinations)
  
  
  current_neuron_numbers = [-1 for _ in range(layers_used)]
  for i in range(layers_used):
    current_neuron_numbers[i], previous_combinations = pic_value(params.neuron_numbers, parameters_genereted, previous_combinations)

  save_state(parameters_genereted, next_layers_used)
  
  return mount_command(current_num_layers, current_density, current_network_type, current_train_algorithm, current_error_function, current_neuron_numbers, current_activation_function)
  
