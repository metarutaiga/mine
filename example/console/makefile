# Generated from ChatGPT (GPT-4 Omni)

CXX := g++
CXXFLAGS := -O3 --std=c++20 -I../.. -I../../format -fdata-sections -ffunction-sections
LDFLAGS := -Wl,-dead_strip

SRC_DIRS := ../../format/coff ../../syscall ../../syscall/windows ../../x86
BUILD_DIR := build
BIN := mine

# Find all .cpp files recursively in source dirs
SOURCES := $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS))) main.cpp

# Generate corresponding .o paths in build/ directory
OBJECTS := $(foreach src,$(SOURCES),$(BUILD_DIR)/$(subst .,-,$(subst /,+,$(basename $(src)))).o)

# Rule to build the binary
$(BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

# Rule to compile each .cpp to its mirrored .o path
$(BUILD_DIR)/%.o:
	@mkdir -p $(dir $@)
	$(eval SRC := $(notdir $(basename $@)))
	$(eval SRC := $(subst -,.,$(SRC)))
	$(eval SRC := $(subst +,/,$(SRC)).cpp)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $@

# Default target
all: $(BIN)

# Clean rule
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
