# My Vim Editor

A lightweight terminal-based text editor written in C, inspired by vim.

## Features
- Syntax highlighting for common programming languages
- Search functionality (Ctrl-F)
- Basic editing operations
- File save/load capabilities
- Status bar with helpful information

## Building
```bash
make        # Build the editor
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Rebuild the project
```

## Usage
```bash
./my_vim [filename]  # Open a file (or create new if it doesn't exist)
```

### Key Bindings
- Ctrl-S: Save file
- Ctrl-Q: Quit
- Ctrl-F: Find in file
- Arrow keys: Move cursor
- Page Up/Down: Scroll through file
- Home/End: Move to start/end of line

## Project Structure
- src/: Source files
- inc/: Header files
- objs/: Object files (created during build)

## Contributing
Feel free to submit issues and enhancement requests.