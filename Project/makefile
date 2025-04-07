init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	clang -Wall -o treasure_manager src/treasure_manager.c src/cmd_args_parser/cmd_args_parser.c src/operations/operations.c
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
	./treasure_manager --help

add:
	@echo "Adding new treasure hunt"
	./treasure_manager --add hunt01

ls:
	@echo "Listing treasure hunts"
	./treasure_manager --list hunt01

rm_h:
	@echo "Removing treasure hunt"
	./treasure_manager --remove hunt01

rm_t:
	@echo "Removing treasure"
	./treasure_manager --remove hunt01 treasure01
