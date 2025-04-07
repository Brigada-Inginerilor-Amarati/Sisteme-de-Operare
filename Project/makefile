init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	clang -Wall -o treasure_manager src/treasure_manager.c src/cmd_args_parser/cmd_args_parser.c src/operations/operations.c
	@echo "Project built successfully"

clean:
	@echo "Cleaning project"
	rm -rf build/*

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

list:
	@echo "Listing treasure hunts"
	./treasure_manager --list hunt01

remove_hunt:
	@echo "Removing treasure hunt"
	./treasure_manager --remove hunt01

remove_treasure:
	@echo "Removing treasure"
	./treasure_manager --remove hunt01 treasure01
