# binaries
BIN = bin
BIN_MANAGER = treasure_manager
BIN_SHELL = treasure_hub
BIN_MONITOR = treasure_monitor

# compiler + flags
CC = clang
CFLAGS = -Wall

# main source code
SRC_MANAGER = src/treasure_manager/treasure_manager.c
SRC_SHELL = src/treasure_hub/shell/shell.c
SRC_MONITOR = src/treasure_hub/monitor/monitor.c

# libraries
SRC_LIB_MANAGER = $(shell find src/lib/treasure_manager -name "*.c")
SRC_LIB_HUB = $(shell find src/lib/treasure_hub/shell -name "*.c")
SRC_LIB_MONITOR = $(shell find src/lib/treasure_hub/monitor -name "*.c")
SRC_LIB_GLOBAL = $(shell find src/lib/global_utils -name "*.c")

init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	@echo "\nBuilding treasure_manager"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MANAGER) $(SRC_LIB_MANAGER) $(SRC_MANAGER) $(SRC_LIB_GLOBAL)
	@echo "\nBuilding treasure_hub shell"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_SHELL) $(SRC_LIB_HUB) $(SRC_SHELL) $(SRC_LIB_GLOBAL)
	@echo "\nBuilding treasure_hub monitor"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MONITOR) $(SRC_LIB_MONITOR) $(SRC_MONITOR) $(SRC_LIB_GLOBAL)
	@echo "\nProject built successfully"

clean:
	@echo "Cleaning project"
	rm -rf treasure_hunts
	rm -rf logs
	mkdir treasure_hunts
	mkdir logs

.PHONY: all build clean init help add list remove_hunt remove_treasure
