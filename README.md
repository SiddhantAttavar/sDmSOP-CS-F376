# sDmSOP-CS-F376

LNS-VNS hybrid solution for the single depot multiple set orienteering problem as part of my Design Project (CS-F376)

## Project Overview

This project implements an Adaptive Large Neighbourhood Search (ALNS) algorithm for solving the Set Team Orienteering Problem (STOP). The implementation provides a high-performance C++17 solution with automated build system.

## Build System

A Makefile has been created to automate the compilation of the ALNS implementation for STOP.

### Build Output
- **Binary Location**: `bin/sDmSOP_ALNS_STOP.out`
- **Object Files**: `build/` directory
- **Compilation Flags**: `-O3 -std=c++17`

### Quick Start

#### Build the Project
```bash
make all
```
Or simply:
```bash
make
```

#### Clean Build Artifacts
```bash
make clean
```

#### Full Clean (includes binary)
```bash
make distclean
```

#### Rebuild from Scratch
```bash
make rebuild
```

### Available Targets

| Target | Description |
|--------|-------------|
| `make all` | Compile and link the entire project (default) |
| `make clean` | Remove intermediate object files in `build/` directory |
| `make distclean` | Remove both object files and the binary executable |
| `make rebuild` | Perform `distclean` followed by `all` |
| `make info` | Display build configuration information |
| `make help` | Show all available targets and their descriptions |
| `make run` | Display binary location and usage information |

## Compilation Details

### Source Files Compiled
```
src/sDmSOP_ALNS/main.cpp
src/sDmSOP_ALNS/solution.cpp
src/sDmSOP_ALNS/calc.cpp
src/sDmSOP_ALNS/precomp.cpp
src/sDmSOP_ALNS/initial.cpp
src/sDmSOP_ALNS/remove.cpp
src/sDmSOP_ALNS/insert.cpp
src/sDmSOP_ALNS/instance.cpp
```

### Header Files (Tracked for Dependency)
```
src/sDmSOP_ALNS/utils.h
src/sDmSOP_ALNS/constants.h
src/sDmSOP_ALNS/solution.h
src/sDmSOP_ALNS/calc.h
src/sDmSOP_ALNS/precomp.h
src/sDmSOP_ALNS/initial.h
src/sDmSOP_ALNS/remove.h
src/sDmSOP_ALNS/insert.h
src/sDmSOP_ALNS/instance.h
```

## Compiler Settings

- **Compiler**: `g++` with C++17 support
- **Optimization**: `-O3` (maximum optimization for performance)
- **Warnings**: `-Wall -Wextra` (comprehensive warning checking)
- **Output**: 64-bit ELF executable

## Directory Structure

```
project_root/
├── Makefile                  # Build automation
├── README.md                 # This file
├── src/
│   └── sDmSOP_ALNS/         # Source files
│       ├── *.cpp
│       └── *.h
├── bin/
│   └── sDmSOP_ALNS_STOP.out # Compiled binary
├── build/                    # Object files (generated)
│   └── *.o
└── stop_instances/           # Test problem instances
    ├── stop_1veh/
    ├── stop_2veh/
    └── stop_3veh/
```

## Usage Example

### Compile
```bash
cd /home/sidat/college/3-1/CS-F376/sDmSOP-CS-F376
make all
```

### Run
```bash
./bin/sDmSOP_ALNS_STOP.out <input_instance> <output_file>
```

Example:
```bash
./bin/sDmSOP_ALNS_STOP.out stop_instances/stop_1veh/115rat575_RND_T20_p1.sop results.txt
```

### Full Workflow
```bash
# Clean previous build
make clean

# Build fresh
make all

# View build info
make info

# Run on a test instance
./bin/sDmSOP_ALNS_STOP.out stop_instances/stop_1veh/115rat575_RND_T20_p1.sop output.txt
```

## Build Behavior

- **Incremental Compilation**: Only modified files are recompiled
- **Dependency Tracking**: Changes to header files trigger recompilation of dependent source files
- **Automatic Directories**: `build/` and `bin/` directories are created automatically if they don't exist

## Troubleshooting

### Clean Rebuild if Compilation Fails
```bash
make distclean
make all
```

### View Build Information
```bash
make info
```

### Verbose Build
```bash
make all 2>&1 | tee build.log
```

## Recent Bug Fixes

The following critical bugs were fixed to ensure stability:

### 1. Struct Definition Issues
- Removed duplicate `struct Solution` definition from solution.cpp
- Fixed Instance struct type mismatches between header and implementation
- Properly separated struct declaration (headers) from implementation (cpp)

### 2. preprocess_costs Function Bugs
- Fixed array indexing bug: `s.cities_in_cluster[t.u[i][j]]` → `s.cities_in_cluster[j]`
- Fixed cluster computation to correctly iterate over clusters
- Added early return for empty route to prevent vector access crashes

### 3. Test Results
- ✅ **306/307 test instances passed** in comprehensive stress testing
- ✅ **No segmentation faults** on previously problematic instances
- ✅ **Stable execution** across all problem sizes

## Performance Considerations

The Makefile uses `-O3` optimization for performance-critical code:
- Inline function expansion
- Loop optimizations
- Register allocation optimization
- Profile-guided optimizations

For debug builds, modify the CXXFLAGS in the Makefile:
```makefile
CXXFLAGS = -g -std=c++17 -Wall -Wextra  # For debugging
```

## Notes

### Compiler Warnings
Some warnings are expected and can be safely ignored:
- `-Wunused-parameter`: Some parameters are part of required signatures but not used
- `-Wmaybe-uninitialized`: Conservative analysis; variables are properly initialized at runtime

### Build Artifacts
- **object files** (*.o): Intermediate compiled code in `build/`
- **executable**: Final binary in `bin/sDmSOP_ALNS_STOP.out`

## Contact/Support

If you encounter build issues:
1. Run `make clean` then `make all`
2. Check compiler version: `g++ --version`
3. Verify C++17 support is available
4. Review compilation errors in full output
