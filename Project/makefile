# binaries
BIN = bin
BIN_MANAGER = treasure_manager
BIN_SHELL = treasure_hub
BIN_MONITOR = treasure_monitor
BIN_CALCULATOR = treasure_calculator

# compiler + flags
CC = clang
CFLAGS = -Wall

# main source code
SRC_MANAGER = src/treasure_manager/treasure_manager.c
SRC_SHELL = src/treasure_hub/shell/shell.c
SRC_MONITOR = src/treasure_hub/monitor/monitor.c
SRC_CALCULATOR = src/treasure_hub/calculate_scores/calculate_scores.c

# libraries
SRC_LIB_MANAGER = $(shell find src/treasure_manager/lib_manager -name "*.c")
SRC_LIB_SHELL = $(shell find src/treasure_hub/shell/lib_shell -name "*.c")
SRC_LIB_MONITOR = $(shell find src/treasure_hub/monitor/lib_monitor -name "*.c")
SRC_LIB_CALCULATOR =  $(shell find src/treasure_hub/calculate_scores/calculate_scores -name "*.c")
SRC_LIB_GLOBAL = $(shell find src/lib -name "*.c")

init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	@echo "\nBuilding treasure_manager"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MANAGER) $(SRC_LIB_GLOBAL) $(SRC_LIB_MANAGER) $(SRC_MANAGER)
	@echo "\nBuilding treasure_hub shell"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_SHELL) $(SRC_LIB_GLOBAL) $(SRC_LIB_SHELL) $(SRC_SHELL)
	@echo "\nBuilding treasure_hub monitor"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MONITOR) $(SRC_LIB_GLOBAL) $(SRC_LIB_MONITOR) $(SRC_MONITOR)
	@echo @echo "\nBuilding treasure_hub calculator"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_CALCULATOR) $(SRC_LIB_GLOBAL) $(SRC_LIB_CALCULATOR) $(SRC_CALCULATOR)
	@echo "\nProject built successfully"
	bin/treasure_hub

manager:
	@echo "Adding new element to treasure hunt"
	bin/treasure_manager --add hunt01

clean:
	@echo "Cleaning project"
	rm -rf treasure_hunts
	rm -rf logs
	mkdir treasure_hunts
	mkdir logs
