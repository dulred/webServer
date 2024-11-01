# Makefile for combined_project

# Set the CMake executable path
CMAKE := cmake

# Set the build directory
BUILD_DIR := build

all: auto_build

clean:
	rm -rf $(BUILD_DIR)

auto_build:
	mkdir $(BUILD_DIR) && cd $(BUILD_DIR) && $(CMAKE) -G "Unix Makefiles" .. && make
