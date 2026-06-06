import sqlite3 as sql
import get_params as parse_params
import get_results as parse_result

def database_setup(db_path="./database/experiment_records.db"):
  connection = sql.connect(db_path)
  
  connection.execute("PRAGMA journal_mode=WAL;")
  connection.execute("PRAGMA synchronous = NORMAL;")
  connection.execute("PRAGMA foreign_keys = ON;")
  connection.execute("PRAGMA cache_size = -64000;")
  connection.execute("PRAGMA locking_mode = EXCLUSIVE;")

  return connection

def add_record(conn, res, params):
    parameters = parse_params.parse_parameters_string(params)
    parseador = parse_result.WorkerResultParser()
    results = parseador.parse_result(res)
    cursor = conn.cursor()

    cursor.execute("BEGIN")
    # ENVIO DE QUERIES
  
    # 1. Inserção do Experimento
    query_experiments = """
        INSERT INTO experiment (
            num_layers, density, train_algorithm, network_type, error_function, 
            seed, version, parameter_hash, total_parameters, train_time_nanoseconds, 
            train_mse_error, train_bit_error, test_time_nanoseconds, test_mse_error, 
            test_bit_error, data_points_needed
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    """
    
    experiment_data = (
        parameters.num_layers  % 2**32, 
        parameters.sparsity, 
        parameters.train_algorithm  % 2**32, 
        parameters.network_type  % 2**32, 
        parameters.error_function  % 2**32, 
        parameters.random_seed  % 2**32,
        0, 
        results.hash_id  % 2**32, 
        results.total_parameters  % 2**32, 
        results.train_time  % 2**32, 
        results.train_mse_error, 
        results.train_bit_error  % 2**32, 
        results.test_time % 2**32, 
        results.test_mse_error, 
        results.test_bit_error  % 2**32, 
        results.data_points_needed  % 2**32
    )
    
    cursor.execute(query_experiments, experiment_data)

    # 2. Inserção das Camadas
    exp_id = cursor.lastrowid
    query_layers = """
        INSERT INTO layers (
            experiment_id, layer_index, num_neurons, activation_function
        ) VALUES (?, ?, ?, ?)
    """
    
    for i in range(parameters.num_layers):
        layer_data = (
            exp_id,
            i,
            parameters.neurons_by_layer[i],
            parameters.activation_by_layer[i]
        )
        cursor.execute(query_layers, layer_data)
    
    # FIM DOS QUERIES
    cursor.execute("END")
    conn.commit()
    cursor.close()
