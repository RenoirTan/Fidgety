# Configurations
DIST_DIR ?= dist/
BUILD_DIR ?= build/
SRC_DIR ?= ./

# Commands
CMAKE ?= cmake
CTEST ?= env --chdir=$(BUILD_DIR) ctest

# Flags
CMAKE_GENERATE_FLAGS ?= -B $(BUILD_DIR) -S $(SRC_DIR) $(CMAKE_GENERATE_SUPPFLAGS)
CMAKE_BUILD_FLAGS ?= --build $(BUILD_DIR) $(CMAKE_BUILD_SUPPFLAGS)
CMAKE_TEST_FLAGS ?= --progress --verbose $(CMAKE_TEST_SUPPFLAGS)
CMAKE_INSTALL_FLAGS ?= --install $(BUILD_DIR) --prefix $(DIST_DIR) $(CMAKE_INSTALL_SUPPFLAGS)

.PHONY: generate build setup_test_env test install clean_build clean_dist clean

generate: CMakeLists.txt
	$(CMAKE) $(CMAKE_GENERATE_FLAGS)

build: generate
	$(CMAKE) $(CMAKE_BUILD_FLAGS)

setup_test_env:
	mkdir -p tmp/tests/decoder
	mkdir -p tmp/tests/encoder

test: build setup_test_env
	$(CTEST) $(CMAKE_TEST_FLAGS)

install: build
	$(CMAKE) $(CMAKE_INSTALL_FLAGS)

clean_build:
	rm -rf $(BUILD_DIR)
	rm -f include/fidgety/config.h

clean_dist:
	rm -rf $(DIST_DIR)

clean: clean_build clean_dist
