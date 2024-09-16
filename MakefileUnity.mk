# Makefile para executar testes localmente

# Tool definitions
CC ?= gcc
CXX ?= g++

# Settings
SRC_DIR = ./src
INC_DIR = ./include
TEST_DIR = ./tests
BUILD_DIR = .

UNITY_HOME = ./tools/Unity
NAME = app.elf

# Search path for header files
CFLAGS += -I$(UNITY_HOME)/src
CFLAGS += -I$(UNITY_HOME)/extras/fixture/src
CFLAGS += -I$(INC_DIR)/Button
CFLAGS += -I$(INC_DIR)/Synth


# List module source files
CSOURCES =  $(UNITY_HOME)/src/unity.c
CSOURCES += $(UNITY_HOME)/extras/fixture/src/unity_fixture.c
CSOURCES += $(wildcard $(TEST_DIR)/*/*.c)
CSOURCES += $(wildcard $(SRC_DIR)/*/*.c)

#Symbols
#Remover função de detecção de vazamento de memória decorrente do uso de memória dinâmica
SYMBOLS=-DUNITY_FIXTURE_NO_EXTRAS

# Compiler flags
CFLAGS += -Wall -ffunction-sections 

# Linker flags
LDFLAGS += -lm

# Generate names for output object files (*.o)
COBJECTS = $(patsubst %.c, %.o, $(CSOURCES))

# Default rule: build application
.PHONY: all
all: $(NAME)
	./$(NAME)

# Build components
$(COBJECTS) : %.o : %.c
%.o: %.c
	$(CC) $(CFLAGS) $(SYMBOLS) -c $< -o $@

# Build the target application
.PHONY: $(NAME)
$(NAME): $(COBJECTS)
	$(CC) $(COBJECTS) -o $(BUILD_DIR)/$(NAME) $(LDFLAGS)

# Remove compiled object files
.PHONY: clean
clean:
	rm -f $(COBJECTS)
	rm -f $(NAME)

# Run tests
.PHONY: test
test:
	make -C $(TEST_DIR)
	
# Clean tests
.PHONY: test_clean
test_clean:
	make -C $(TEST_DIR) clean