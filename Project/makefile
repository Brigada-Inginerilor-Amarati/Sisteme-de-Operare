BIN = bin
BIN_MANAGER = treasure_manager
BIN_HUB = treasure_hub
BIN_MONITOR = monitor
CC = clang
CFLAGS = -Wall
SRC_MANAGER = $(shell find src/treasure_manager -name "*.c")
SRC_HUB = src/treasure_hub/shell.c
SRC_MONITOR = src/treasure_hub/monitor.c
SRC_UTILS = $(shell find src/lib -name "*.c")

init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MANAGER) $(SRC_UTILS) $(SRC_MANAGER)
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_HUB) $(SRC_UTILS) $(SRC_HUB)
	$(CC) $(CFLAGS) -o $(BIN)/$(BIN_MONITOR) $(SRC_UTILS) $(SRC_MONITOR)
	@echo "Project built successfully"

usage:
	@echo "Available commands:"
	@echo "init: Initialize project"
	@echo "build: Build project"
	@echo "clean: Clean project"
	@echo "help: Display help"
	@echo "add: Add new treasure hunt"
	@echo "list: List treasure hunts"
	@echo "remove_hunt: Remove treasure hunt"
	@echo "remove_treasure: Remove treasure"

help:
	$(BIN)/$(BIN_MANAGER) --help

add:
	@echo "Adding new treasure hunt"
	$(BIN)/$(BIN_MANAGER) --add hunt01

ls_h:
	@echo "Listing treasure hunts"
	$(BIN)/$(BIN_MANAGER) --list hunt002

ls_t:
	@echo "Listing treasures"
	$(BIN)/$(BIN_MANAGER) --list hunt01 69

rm_h:
	@echo "Removing treasure hunt"
	$(BIN)/$(BIN_MANAGER) --remove hunt002

rm_t:
	@echo "Removing treasure"
	$(BIN)/$(BIN_MANAGER) --remove hunt01 69

clean:
	@echo "Cleaning project"
	rm -rf treasure_hunts
	rm -rf logs
	mkdir treasure_hunts
	mkdir logs

.PHONY: all build clean init help add list remove_hunt remove_treasure
