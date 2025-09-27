# Build and Development

## Requirements

Before building this project, make sure the following tools are available:

- meson >= 1.9.1
- clang >= 3.0 (or any compiler that supports C11 and its clang extensions standards)

Optional (for enhanced documentation)

- doxygen -- to generate project documentation
- graphviz -- to generate call graphs and class diagrams
- python >= 3.6 -- required for the Doxygen setup script

## Build Type Configuration

| Build Type     | Optimization Level | Debugging Information | Predefined Macro |
| -------------- | ------------------ | --------------------- | ---------------- |
| debug          | -O0                | -g                    | DEBUG            |
| debugoptimized | -O2                | -g                    | DEBUG            |
| release        | -O3                | no                    | NDEBUG           |
| minsize        | -Os                | no                    | NDEBUG           |

## Build Options

### debug_trace_execution

- **Description**: Enable execution tracing debug output
- **Default**: false (but automatically enabled in debug builds unless explicitly set to false)
- **Debug builds**: Enabled by default (unless explicitly set to false)
- **Release builds**: Disabled by default

## Build Commands

### Initial Setup

```bash
# Create build directory
meson setup build

# Or with specific build type
meson setup build --buildtype=debug
```

### Configuration

```bash
# Configure build options
meson configure build -Ddebug_trace_execution=true  # or false
meson configure build -Dbuildtype=debug             # or debugoptimized/release/minsize

# View current configuration
meson configure build
```

### Building

```bash
# Build the project
meson compile -C build

# Build with verbose output
meson compile -C build -v
```

### Installation

```bash
# Install to system (not recommended for development)
meson install -C build
```

### Cleaning

```bash
# Clean build artifacts
meson compile -C build --clean

# Full rebuild (delete build directory and start fresh)
rm -rf build
meson setup build
```

## Running the Compiler

After building, you can run the compiler:

```bash
# Run the clox compiler
./build/clox

# Run with a Lox source file
./build/clox example.lox
```

## Development Workflow

### Debug Build

```bash
meson setup build --buildtype=debug
meson compile -C build
./build/clox
```

### Release Build

```bash
meson setup build --buildtype=release
meson compile -C build
./build/clox
```

### Testing Configuration Changes

```bash
# Enable execution tracing for debugging
meson configure build -Ddebug_trace_execution=true
meson compile -C build
./build/clox
```

## Documentation Generation

The project includes Doxygen-based API documentation with enhanced styling. There are two ways to generate documentation:

### Using the Setup Script (Recommended)

The project provides an automated setup script that handles all dependencies and configuration:

```bash
# Generate documentation with automatic setup
python tools/setup_docs.py

# The documentation will be generated in docs/doxygen/html/
# Open docs/doxygen/html/index.html in your browser
```

### Viewing Documentation

After generation, open [docs/doxygen/html/index.html](./doxygen/html/index.html) in a web browser to browse the documentation.

### Rebuilding Documentation

To update documentation after code changes:

```bash
# Using the setup script (recommended)
python tools/setup_docs.py

# Or manually delete and regenerate
rm -rf docs/doxygen/html
python tools/setup_docs.py
```
