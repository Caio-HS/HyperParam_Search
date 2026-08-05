# Nome do executavel final
TARGET = worker

# Compilador
CC = gcc

# Diretorios
SRC_DIR = src
INC_DIR = include
OBJ_BASE_DIR = obj
DEP_DIR = dep
OPTIMIZATION_REPORTS_DIR = reports/optimization

# --- Flags de Compilacao ---

# Flags comuns a ambos os modos
COMMON_FLAGS = -I$(INC_DIR)

# Define o padrao C e pedancia estrita
C_STD_FLAGS = -std=c2x -pedantic

# Flags de Debug
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
              -Wanalyzer-too-complex --param=analyzer-max-enodes-per-program-point=260000 \
              -fsanitize=address,undefined,leak -fstack-protector-strong 


# Flags de Execucao (Release)
RELEASE_FLAGS = -O3 -march=native -flto -DNDEBUG -Wall \
                -fopt-info-all=$(OPTIMIZATION_REPORTS_DIR)/comp_report.txt

# Flags do Linker
LDFLAGS = -O3 -lfann -lm -lxxhash -fopt-info-all=$(OPTIMIZATION_REPORTS_DIR)/link_report.txt

# --- Logica de Selecao de Modo ---

# Define 'release' como padrao
MODE ?= release

ifeq ($(MODE), debug)
    CFLAGS = $(COMMON_FLAGS) $(C_STD_FLAGS) $(DEBUG_FLAGS)
    OBJ_DIR = $(OBJ_BASE_DIR)/debug
    LDFLAGS += -fsanitize=address,undefined,leak
else
    CFLAGS = $(COMMON_FLAGS) $(RELEASE_FLAGS)
    OBJ_DIR = $(OBJ_BASE_DIR)/release
    LDFLAGS += -O3 -flto
endif

# --- Arquivos ---

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPFILES = $(SOURCES:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

# --- Regras ---

all: $(DEPFILES) $(TARGET) 

# Compilacao do executavel
$(TARGET): $(OBJECTS)
	@echo "[LINK] Gerando executável: $@"
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilacao dos objetos em pastas especificas
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "[CC]   Compilando $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Criacao dos diretorios
$(OBJ_DIR):
	@echo "[INFO] Inicializando estrutura de diretorios..."
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OPTIMIZATION_REPORTS_DIR)
	@mkdir -p $(DEP_DIR)

# Geracao dependencias
$(DEP_DIR)/%.d: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "[DEP]  Gerando dependências de $<"
	@$(CC) $(CFLAGS) -MM -MT $< -MF $@ $<



# Atalhos praticos
sao_pedro:
	@$(MAKE) MODE=debug

debug:
	@$(MAKE) MODE=debug

release:
	@$(MAKE) MODE=release

clean:
	@echo "[CLEAN] Removendo arquivos gerados..."
	@rm -rf $(OBJ_BASE_DIR) $(TARGET) $(OPTIMIZATION_REPORTS_DIR) $(DEP_DIR)

.PHONY: all debug sao_pedro release clean
