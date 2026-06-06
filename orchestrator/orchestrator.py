import subprocess as proc
import struct

import database as db
import parameters_generator as params_gen

WORKER_NAME = "../worker"
RETURN_FORMAT = "QQQQQfQQfQ"
RETURN_SIZE = 72

conn = db.database_setup()

while True:

    parameters = params_gen.next_params()
    cmd = WORKER_NAME + parameters
    p = proc.Popen(cmd, stdout=subprocess.PIPE)

    output, errors_outputed = p.communicate()

    returned = p.returncode
    
    if returned is not 0:
        print(f"Worker pid: {work.p.pid}, parameters: {work.parameters}, returned: {returned}")

    raw_result = p.stdout.read()
#    res = struct.unpack(RETURN_FORMAT, raw_result[:RETURN_SIZE])
    db.add_record(conn, raw_results, parameters)



