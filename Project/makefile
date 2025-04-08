
EXE=./treasure_manager

init:
	@echo "Initializing project"
	mkdir treasure_hunts
	mkdir logs

build:
	@echo "Building project"
	clang -Wall -o ${EXE} src/treasure_manager.c src/cmd_args_parser/cmd_args_parser.c src/operations/operations.c src/treasure/treasure.c
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
	${EXE} --help

add:
	@echo "Adding new treasure hunt"
	${EXE} --add hunt01

ls:
	@echo "Listing treasure hunts"
	${EXE} --list hunt01

rm_h:
	@echo "Removing treasure hunt"
	${EXE} --remove hunt01

rm_t:
	@echo "Removing treasure"
	${EXE} --remove hunt01 treasure01
