# Makefile for sDmSOP ALNS implementation
# Compiles the Adaptive Large Neighbourhood Search algorithm for the Set Team Orienteering Problem

# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -std=c++20 -march=native -flto
LDFLAGS = 

# Directories
SRC_DIR = src/sDmSOP_ALNS
BIN_DIR = bin
BUILD_DIR = build

# Output binary
TARGET = $(BIN_DIR)/sDmSOP_ALNS_STOP.out

# Source files (header-only files are not included in compilation)
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/solution.cpp \
          $(SRC_DIR)/calc.cpp \
          $(SRC_DIR)/precomp.cpp \
          $(SRC_DIR)/initial.cpp \
          $(SRC_DIR)/remove.cpp \
          $(SRC_DIR)/insert.cpp \
          $(SRC_DIR)/instance.cpp

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Header files for dependency tracking
HEADERS = $(SRC_DIR)/utils.h \
          $(SRC_DIR)/constants.h \
          $(SRC_DIR)/solution.h \
          $(SRC_DIR)/calc.h \
          $(SRC_DIR)/precomp.h \
          $(SRC_DIR)/initial.h \
          $(SRC_DIR)/remove.h \
          $(SRC_DIR)/insert.h \
          $(SRC_DIR)/instance.h

# Default target
.PHONY: all
all: $(TARGET)

# Create directories
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Build successful! Binary: $@"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Clean everything including binary
.PHONY: distclean
distclean: clean
	@echo "Removing binary..."
	rm -f $(TARGET)
	@echo "Distclean complete"

# Rebuild from scratch
.PHONY: rebuild
rebuild: distclean all

# Run the binary with sample arguments (requires input file)
.PHONY: run
run: $(TARGET)
	@echo "Binary ready at: $(TARGET)"
	@echo "Usage: $(TARGET) <input_file> <output_file>"

# Print build information
.PHONY: info
info:
	@echo "sDmSOP ALNS Build Information"
	@echo "=============================="
	@echo "Source directory: $(SRC_DIR)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Binary directory: $(BIN_DIR)"
	@echo "Target: $(TARGET)"
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Source files: $(SOURCES)"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make all       - Build the project (default)"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make distclean - Remove build artifacts and binary"
	@echo "  make rebuild   - Clean and rebuild from scratch"
	@echo "  make info      - Display build information"
	@echo "  make help      - Show this help message"
