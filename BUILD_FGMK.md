# **`fgmk`**

This markdown document describes how to use `fgmk` to streamline the Fidgety
build process.

## Configurations

Due to availability of some libraries in different distributions, Fidgety may
have to download libraries, or explicitly set the version of the relevant
dependencies when generating the CMake build folder. These options can be
specified using `fgmk` or using the provided Makefile but to save you some
hassle, here are some commands you can run to setup Fidgety for your specific
distribution.

The following commands assume you are running as a regular user in the project
root and you want to build the unit tests.

### Arch Linux

```shell
# Dependencies (using qt6)
sudo pacman -Syu \
    boost \
    boost-libs \
    spdlog \
    fmt \
    nlohmann_json \
    qt5-base

__cpu=$(nproc)
__mem=$(($(cat /proc/meminfo | grep "MemTotal" | awk '{ print $2 }') / 2000000))
command -v nproc &>/dev/null && ./fgmk s parallel $((__cpu < __mem ? __cpu : __mem))
```

### Debian, Ubuntu

```shell
# Dependencies (using qt5)
sudo apt update
sudo apt install \
    libboost-all-dev \
    libspdlog-dev \
    libfmt-dev \
    nlohmann-json3-dev \
    qtbase5-dev

__cpu=$(nproc)
__mem=$(($(cat /proc/meminfo | grep "MemTotal" | awk '{ print $2 }') / 2000000))
command -v nproc &>/dev/null && ./fgmk s parallel $((__cpu < __mem ? __cpu : __mem))
```

### Fedora

```shell
# Dependencies (using qt5)
sudo dnf install \
    boost \
    spdlog-devel \
    fmt-devel \
    qt5-qtbase

__cpu=$(nproc)
__mem=$(($(cat /proc/meminfo | grep "MemTotal" | awk '{ print $2 }') / 2000000))
command -v nproc &>/dev/null && ./fgmk s parallel $((__cpu < __mem ? __cpu : __mem))
./fgmk s fidgety_nlohmannjson_from_source on
```

If you don't want to build any tests, run:

```shell
./fgmk s build_testing off
```

## Subcommands

`fgmk` has 8 subcommands for editing build configurations and building Fidgety.

| Subcommand | Description |
| ---------- | ----------- |
| [`report`](#report) | Report on `fgmk`'s inner workings, like the available config options. |
| [`get`](#get) | Get the value of a certain config key. |
| [`set`](#set) | Set the value of a certain config key. |
| [`generate`](#generate) | Generate the CMake build folder. |
| [`build`](#build) | Build Fidgety. |
| [`install`](#install) | Install Fidgety to an install prefix. |
| [`test`](#test) | Run the Fidgety's unit tests. |
| [`clean`](#clean) | Clean the install and build directories. |

These subcommands have aliases which are short forms of the subcommands they
represent. ```fgmk --help``` shows what short forms each subcommand has.

### `report`

Usage:

```shell
./fgmk report <REPORT_ITEM>
```

This subcommand is used to get a look into how `fgmk` functions. `report`
has one positional argument called \<REPORT_ITEM\>, which can be any of the
following:

| REPORT_ITEM | Description |
| ----------- | ----------- |
| `key_to_integral` / `kti` | Get the allowed data type for each config option. |
| `default_config` / `dc` | Get the default configuration used by `fgmk`. |
| `final_config` / `fc` | Get the configuration used by `fgmk` after overriding the default configs with the data from `.fgmk.json`. |

`report` also accepts 2 flags:

| Flags | Description |
| ----- | ----------- |
| `--clean` / `-c` | Do not print unnecessary items. This means that configs whose value is `None` are not shown. |
| `--long` / `-l` | Use a more long-winded format to print out the report. This should help with readability. |

### `get`

Usage:

```shell
./fgmk get <CONFIG_KEY>
```

You can use the `get` subcommand to get the value of a \<CONFIG_KEY\>. To get
a list of available config keys, you can run
```./fgmk report key_to_integral --long``` or refer to the table below.

| Config Key | Data Type | Description |
| ---------- | --------- | ----------- |
| cmake | str | The command to use to call CMake |
| ctest | str | The command to use to call CTest |
| cmake_generate_suppflags | list | Supplementary flags for generating the CMake build folder. |
| cmake_build_suppflags | list | Supplementary flags for build Fidgety. |
| cmake_install_suppflags | list | Supplementary flags when installing Fidgety. |
| cmake_test_suppflags | list | Supplementary flags for running unit tests. |
| cmake_build_type | str | Specify the build type used by CMake. See [this page](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html) for more info. |
| dist_dir | str | Install prefix. |
| build_dir | str | CMake build directory. |
| src_dir | str | The source directory. This should be where the top level CMakeLists.txt should be. |
| build_testing | bool | Whether to build tests. This variable is created by CMake's `enable_testing()`. |
| fidgety_build_testing | bool | Whether to build Fidgety's tests even if it's not the top-level project. `build_testing` must still be turned on for this to work. |
| parallel | int | How many jobs to run in parallel when building binaries. |
| rerun_failed | bool | Whether to tell CTest to rerun failed tests, this is the same as running `ctest --rerun-failed`. |
| output_on_failure | bool | Whether to show all output from a test if it fails, this is the same as running `ctest --output-on-failure`. |
| fidgety_gtest_from_source | bool | Whether to build googletest from source. Turned on by default (as recommended by Google themselves). |
| fidgety_googletest_release_url | str | The URL for the googletest source code zip file. |
| fidgety_fmtlib_from_source | bool | Whether to build fmt from source. |
| fidgety_fmtlib_release_url | str | The URL for the fmt source code zip file. |
| fidgety_fmtlib_header_only | bool | Whether to use the header-only version of fmt. |
| fidgety_spdlog_from_source | bool | Whether to build spdlog from source. |
| fidgety_spdlog_release_url | str | The URL for the spdlog source code zip file. |
| fidgety_nlohmannjson_from_source | bool | Whether to build nlohmann's json library from source. |
| fidgety_nlohmannjson_release_url | str | The URL for the nlohmann/json source code zip file. |
| fidgety_boost_minver | str | Oldest version of Boost that Fidgety is allowed to use. |
| fidgety_qt_major | int | The major version of Qt Fidgety should use. Default: 5, Accepts: 5, 6 |
| fidgety_qt_from_source | bool | Whether to build Qt from source. |
| fidgety_qt_repo_url | str | The URL for Qt's git repo. |
| fidgety_qt_repo_tag | str | The git tag for Qt's source code. |
| fidgety_test_loglevel | str | The spdlog log level that Fidgety's tests should filter in. Allowed values are: critical, error, warn, info, debug, trace. |
| fidgety_build_extensions | bool | Whether to build Fidgety's provided extensions. |
| fidgety_export_cmake_dir | str | Where Fidgety's `.cmake` files should be installed to. CMake refers to these files when you call `find_package`. |
| fidgety_export_lib_dir | str | Where Fidgety's library files (e.g. `.a`, `.so`) should be installed to. |
| fidgety_export_extlib_dir | str | Where Fidgety's extension library files should be installed to. |
| fidgety_exception_what_use_description | bool | Whether to use `Fidgety::Exception::getLongDescription` to override `std::exception::what`. |
| microsoft_bullshit | bool | Turn on *"patches"* for intellisense. |

### `set`

Usage:

```shell
./fgmk set <CONFIG_KEY> [<CONFIG_VALUE> ...]
```

You can set the value of a certain config key using this subcommand. To get the
available config options, you can refer to the table in [`get`](#get).

If the config option accepts a list, you can pass in more than one config value.

If you want to reset an option back to its default behaviour, you can omit the
config value part, instead only passing these arguments to the terminal:

```shell
./fgmk set dist_dir
```

Due to the current implementation of argparse, you cannot pass an argument
with a hyphen at the front (like in `-DBUILD_TESTING=OFF`) to \<CONFIG_VALUE\>
as is. To circumvent this limitation, you can add a backslash in front of the
argument:

```shell
./fgmk set cmake_generate_suppflags "\-DBUILD_TESTING=OFF"
```

This sets `dist_dir` back to "dist/".

### `generate`

Usage:

```shell
./fgmk generate
```

Generate the CMake build folder. Same as running

```shell
mkdir build/
cd build/
cmake ..
```

### `build`

Usage:

```shell
./fgmk build
```

Tell CMake to build Fidgety.

### `install`

Usage:

```shell
./fgmk install
```

Install Fidgety to `dist_dir`.

### `test`

Usage:

```shell
./fgmk test
```

Run Fidgety's unit tests.

### `clean`

Usage:

```shell
./fgmk clean
```

Delete the build folder.
