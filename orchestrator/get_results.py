import struct
from collections import namedtuple

# Definição de uma estrutura nomeada para facilitar o acesso no Orquestrador
ExecutionResult = namedtuple('ExecutionResult', [
    'total_parameters',
    'data_points_needed',
    'train_time',
    'train_bit_error',
    'test_bit_error',
    'train_mse_error',
    'test_time',
    'test_mse_error',
    'hash_id',          # Caso você envie um hash (ex: xxhash) do modelo/parâmetros
    'status_code'       # Um indicador de sucesso/erro interno do worker
])

class WorkerResultParser:
    def __init__(self):
        # FORMATO ATUAL: "QQQQQfQQfQ" 
        # IMPORTANTE: O caractere '=' ou '@' no início define as regras de alinhamento.
        # Se o C gravou a struct pura via fwrite(&results, sizeof(RESULTS), 1, stdout),
        # o alinhamento nativo com padding do compilador deve ser respeitado usando '@'.
        self.format_string = "QQQQQfQQfQ"
        self.expected_size = 72 # Confirmado pelo sizeof(RESULTS) se houver padding

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



