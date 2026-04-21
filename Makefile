# Nome do executável final
TARGET = worker

# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_BASE_DIR = obj

# --- Flags de Compilação ---

# Flags comuns a ambos os modos
COMMON_FLAGS = -I$(INC_DIR)

# Define o padrão C e pede aderência estrita
C_STD_FLAGS = -std=c2x -pedantic 

# Flags de Debug (conforme solicitado)
DEBUG_FLAGS = -O0 -Wall -Wextra -g -Wformat=2 -Wconversion -Wcast-qual \
              -Wcast-align=strict -Wpointer-arith -Wshadow -Wundef -Wwrite-strings \
              -Wstrict-overflow -Wstrict-aliasing -Wdouble-promotion -Wlogical-op \
              -Wduplicated-cond -Wduplicated-branches -Wjump-misses-init -Walloc-zero \
              -Walloca -Wtrampolines -Wnull-dereference -Wvla -Wmissing-prototypes \
              -Wmissing-declarations -Winline -Wunsafe-loop-optimizations -Wpacked \
              -Wformat-overflow=2 -Wformat-truncation=2 -Wfloat-equal \
              -Woverlength-strings -Wstringop-overflow -Wstringop-truncation \
              -Wimplicit-fallthrough=5 -Wno-aggregate-return -Wbad-function-cast -fanalyzer \
              -Wanalyzer-malloc-leak -Wanalyzer-use-of-uninitialized-value \
              -Wanalyzer-null-dereference -Wanalyzer-double-free -Wanalyzer-tainted-size \
              -Wanalyzer-possible-null-argument -Wvolatile-register-var -Wanalyzer-tainted-divisor \
              -Wanalyzer-tainted-allocation-size -Wanalyzer-double-fclose \
              -Wanalyzer-exposure-through-output-file -Wanalyzer-file-leak \
              -Wanalyzer-use-after-free -Wanalyzer-free-of-non-heap -Wno-infinite-recursion \
              -Wanalyzer-mismatching-deallocation -Wanalyzer-shift-count-negative \
              -Wanalyzer-shift-count-overflow -Wanalyzer-stale-setjmp-buffer \
              -Wanalyzer-unsafe-call-within-signal-handler -Wanalyzer-tainted-array-index \
              -Wanalyzer-use-of-pointer-in-stale-stack-frame -Wanalyzer-write-to-const \
              -Wanalyzer-write-to-string-literal -Wvector-operation-performance \
              -Wanalyzer-too-complex --param=analyzer-max-enodes-per-program-point=260000
#              -fsanitize=address,undefined,leak -fstack-protector-strong

# Flags de Execução (Release)
# -O3: Otimização agressiva
# -march=native: Otimiza para a arquitetura da sua CPU local
# -DNDEBUG: Desativa asserts para performance máxima
RELEASE_FLAGS = -O3 -march=native -flto -DNDEBUG -Wall

# Flags do Linker
LDFLAGS = -lfann -lm -lxxhash

# --- Lógica de Seleção de Modo ---

# Define 'release' como padrão
MODE ?= release

ifeq ($(MODE), debug)
    CFLAGS = $(COMMON_FLAGS) $(C_STD_FLAGS) $(DEBUG_FLAGS)
    OBJ_DIR = $(OBJ_BASE_DIR)/debug
#    LDFLAGS += -fsanitize=address,undefined,leak
else
    CFLAGS = $(COMMON_FLAGS) $(RELEASE_FLAGS)
    OBJ_DIR = $(OBJ_BASE_DIR)/release
    # Adiciona LTO nas flags de linkagem se estiver em release
    LDFLAGS += -O3 -flto
endif

# --- Arquivos ---

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Regras ---

all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilação dos objetos em pastas específicas
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos diretórios de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Atalhos práticos
debug:
	$(MAKE) MODE=debug

release:
	$(MAKE) MODE=release

clean:
	rm -rf $(OBJ_BASE_DIR) $(TARGET)

.PHONY: all debug release clean
