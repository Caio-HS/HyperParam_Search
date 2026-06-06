import sqlite3 as sql
import get_params as parse_params
import get_results as parse_result

def database_setup(db_path="../database/experiment_records.db"):
  connection = sql.connect(db_path)
  
  connection.execute("PRAGMA journal_mode=WAL;")
  connection.execute("PRAGMA synchronous = NORMAL;")
  connection.execute("PRAGMA foreign_keys = ON;")
  connection.execute("PRAGMA cache_size = -64000;")
  connection.execute("PRAGMA locking_mode = EXCLUSIVE;")

  return connection

def add_record(conn, res, params):
  parameters = parse_params.parse_parameters_string(params)
  results = parse_result.parse_result(res)
  cursor = conn.cursor()

  cursor.execute("BEGIN")
  #ENVIO DE QUERRYS
  
  cursor.execute("""INSERT INTO experiments (
                                              num_layers, density, train_algorithm, network_type, error_function, seed, 
                                              version, parameter_hash, total_parameters, train_time_nanoseconds, train_mse_error, train_bit_error, test_time_nanoseconds, test_mse_error, test_bit_error, data_points_needed
                                            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)""", (
    parameters.num_layers, parameters.sparsity, parameters.train_algorithm, parameters.network_type, parameters.error_function, parameters.random_seed,
    0, results.hash_id, results.total_parameters, results.train_time, results.train_mse_error, results.train_bit_error, results.test_time, results.test_mse_error, results.test_bit_error, results.data_points_needed)
                )

  exp_id = cursor.lastrowid
  for i in range(parameters.num_layers):
    cursor.execute("""INSERT INTO layers (
                                          experiment_id, layer_index,
                                          num_neurons, activation_function
                                          ) VALUES (?, ?, ?, ?)
                                          """, (
      exp_id,
      i,
      parameters.neurons_by_layer[i],
      parameters.activation_by_layer[i]
    ))
    
  
  #FIM DOS QUERRYS
  cursor.execute("END")
  cursor.commit()
  cursor.close()



