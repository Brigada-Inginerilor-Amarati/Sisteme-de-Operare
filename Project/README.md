# Treasure Hunt System – Operating Systems Course Project

This project simulates a digital treasure hunt game system on UNIX, involving process management, file operations, and inter-process communication via signals and pipes.

## 🗂️ Project Structure

Project/
├── bin/ # Contains the compiled binaries for the manager, shell and monitor
├── docs/ # Contains project specification PDFs and schematics
├── logs/ # Symlinks to hunt-specific logs (e.g., hunt01.log)
├── src/ # Source code organized by components
│ ├── treasure_manager/ # Phase 1: File-based treasure management tool
│ ├── treasure_hub/ # Phase 2: Interactive shell + monitor process
│ └── lib/ # Shared utilities
├── treasure_hunts/ # Stores all hunt directories and treasure data files
└── makefile # Build and Run Tool

## 📦 Dependencies

This project is designed for Unix-like systems (Linux/macOS) and uses only standard system interfaces.

### 🛠️ Requirements

- **Clang** – C compiler
- **Make** – Build tool for automating compilation (Makefile included)

## 📌 Phase 1 – File Management System

### 🧩 Goal

Implement a program called `treasure_manager` that:

- Creates and manages digital treasure hunts
- Uses binary files to store treasure data with fixed-size records
- Organizes hunts as directories, logging operations in each

### 🛠️ Features

- `--add <hunt_id>` – Add treasure to a hunt (create hunt directory if missing)
- `--list` – List all hunts and treasure counts for each
- `--list <hunt_id>` – List all treasures, showing metadata (file size and last modification time)and entries
- `--list <hunt_id> <treasure_id>` – Display treasure details from selected hunt
- `--remove <hunt_id> <treasure_id>` – Remove a specific treasure
- `--remove <hunt_id>` – Remove the entire hunt directory
- Automatic symlink creation in `logs/` to each `.log` file

### 📁 File Format

Each hunt directory contains:

- A binary file with multiple treasure records: `treasures.bin`

    - `Treasure ID` (string)
    - `Username` (string)
    - `Value` (int)
    - `Latitude`, `Longitude` (float)
    - `Clue` (string)

- A text log file: `log`
- A symlink in the root: `<hunt_id>.log`

## 📌 Phase 2 – Processes and Signals

### 🧩 Goal

Introduce `treasure_hub`, a shell that interacts with a background monitor process using UNIX signals and pipes. The monitor controls access to the `treasure_manager`.

## 📌 Phase 2 – Processes and Signals

### 🧩 Goal

Introduce `treasure_calculator`, a shell addon that creates forks for every hunt in the main directory and calculates the user scores for each.

### 🛠️ Commands (Shell)

- `calculate_scores` – Forks and launches the `treasure_calculator`
- `start_monitor` – Forks and launches the `monitor`
- `list_hunts` – Sends a signal to list available hunts via `treasure_manager`
- `list_treasures <hunt_id>` – Lists all treasures from a specific hunt
- `view_treasure <hunt_id> <treasure_id>` – View detailed info of one treasure
- `stop_monitor` – Gracefully terminates the monitor
- `exit` – Exits shell (only if monitor is not running)
- `clear`, `help` – Auxiliary shell commands

### 🔁 Flow

```text
hub (shell)
 └─ forks monitor
      └─ forks treasure_manager with appropriate flags
```

🧪 Notes
• Fixed-size records ensure predictable binary layout
• Removing treasures implies rewriting the binary file
• Shell disables commands if the monitor hasn’t stopped
• Symlinks are created automatically per hunt in logs/

⚠️ Binary Architecture Notice

The included treasure_manager and treasure_hub binaries are precompiled for ARM64 (e.g., Apple Silicon or ARM-based Linux systems).
If you’re on a different architecture (e.g., x86_64), you must recompile them using: `make build`

👨‍💻 Author

Paul Hondola
University-level Systems Programming Project
