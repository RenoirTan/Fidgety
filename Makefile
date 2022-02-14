# Commands
CMAKE ?= cmake
CTEST ?= ctest

# Configurations
DIST_DIR ?= dist/
BUILD_DIR ?= build/
SRC_DIR ?= ./

# Flags
CMAKE_GENERATE_FLAGS ?= -B $(BUILD_DIR) -S $(SRC_DIR) $(CMAKE_GENERATE_SUPPFLAGS)
CMAKE_BUILD_FLAGS ?= --build $(BUILD_DIR) $(CMAKE_BUILD_SUPPFLAGS)
CMAKE_TEST_FLAGS ?= --test-dir $(BUILD_DIR) --progress --verbose $(CMAKE_TEST_SUPPFLAGS)
CMAKE_INSTALL_FLAGS ?= --install $(BUILD_DIR) --prefix $(DIST_DIR) $(CMAKE_INSTALL_SUPPFLAGS)

generate: CMakeLists.txt
	$(CMAKE) $(CMAKE_GENERATE_FLAGS)

build: generate
	$(CMAKE) $(CMAKE_BUILD_FLAGS)

setup_test_env:
	mkdir -p tmp/tests/{decoder,encoder}

test: build setup_test_env
	$(CTEST) $(CMAKE_TEST_FLAGS)

install: build
	$(CMAKE) $(CMAKE_INSTALL_FLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_DIR)
