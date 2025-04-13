EXE = treasure_manager
CC = clang
CFLAGS = -Wall
SRC = $(shell find src -name "*.c")

init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)
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
	./$(EXE) --help

add:
	@echo "Adding new treasure hunt"
	./$(EXE) --add hunt01

ls_h:
	@echo "Listing treasure hunts"
	./$(EXE) --list hunt002

ls_t:
	@echo "Listing treasures"
	./$(EXE) --list hunt01 69

rm_h:
	@echo "Removing treasure hunt"
	./$(EXE) --remove hunt002

rm_t:
	@echo "Removing treasure"
	./$(EXE) --remove hunt01 69

clean:
	@echo "Cleaning project"
	rm -rf treasure_hunts
	rm -rf logs
	mkdir treasure_hunts
	mkdir logs

.PHONY: all build clean init help add list remove_hunt remove_treasure
