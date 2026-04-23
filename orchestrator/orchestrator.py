import time
import subprocess as proc
import struct
from typing import NamedTuple

import database as db
import parameters_generator as params_gen

active_process = []
MAX_PROCESS = 2
WORKER_NAME = "../worker"
RETURN_FORMAT = "QQQQQfQfQQ"
RETURN_SIZE = 80

class Execution(NamedTuple):
    parameters: string
    p: proc.Popen

conn = db.setup()

while true:

    while (len(active_process) < MAX_PROCESS):
        parameters = params_gen.next_params()
        cmd = WORKER_NAME + parameters
        p = proc.Popen(cmd, stdout=subprocess.PIPE)
        work = Execution(parameters, p)
        active_process.append(work)

    for work in active_process:
        returned = work.p.poll()

        if returned is None:
            continue
            
        if returned is not 0:
            print(f"Worker pid: {work.p.pid}, parameters: {work.parameters}, returned: {returned}")

        active_process.remove(work)
        raw_result = work.p.stdout.read()
        res = struct.unpack(RETURN_FORMAT, raw_result[:RETURN_SIZE])
        db.add_record(conn, res, work.parameters)

    
    time.sleep(1)
