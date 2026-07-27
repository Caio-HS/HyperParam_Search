import struct
from collections import namedtuple

# Definição de uma estrutura nomeada para facilitar o acesso no Orquestrador
ExecutionResult = namedtuple('ExecutionResult', [
    'total_parameters',
    'hash_id',          # Caso você envie um hash (ex: xxhash) do modelo/parâmetros
    'data_points_needed',
    'version',
    'train_time',
    'train_mse_error',
    'train_bit_error',
    'test_time',
    'test_mse_error',
    'test_bit_error'
])

class WorkerResultParser:
    def __init__(self):
        self.format_string = "<IqIHqfIqfI"
        self.expected_size = 50 # Confirmado pelo sizeof(RESULTS) se houver padding

    def parse_result(self, raw_bytes: bytes) -> ExecutionResult:
        """
        Recebe os bytes brutos do stdout do worker, isola o tamanho correto
        e desempacota na estrutura nomeada.
        """
        if not raw_bytes:
            raise ValueError("O worker não retornou nenhum dado no stdout.")
            
        if len(raw_bytes) < self.expected_size:
            raise ValueError(
                f"Tamanho de dados insuficiente. Esperado: {self.expected_size} bytes, "
                f"recebido: {len(raw_bytes)} bytes."
            )

        # Fatia os primeiros 72 bytes para o unpack
        payload = raw_bytes[:self.expected_size]
        
        try:
            unpacked_data = struct.unpack(self.format_string, payload)
            # Retorna o objeto mapeado com nomes legíveis
            return ExecutionResult(*unpacked_data)
        except struct.error as e:
            raise ValueError(f"Erro ao desempacotar estrutura binária do Worker: {e}")



