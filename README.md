# Fidgety

A graphical app that edits config files using toggles, dropdowns, sliders and text entry fields.

This app got its name from those fidget toys with buttons and joysticks.

# Schematic

![Schematic](thats-how-fidgety-works.png)

# Platform Availability

This project was created to address the UI problem in Linux where users are
usually expected to configure their system by editing config files instead of
through a simple GUI as seen on a Windows or MacOS system. Therefore, this
app will have first-class support on Linux and will be developed according to
this policy. You can try and contribute by porting this project to other
operating systems (especially on Windows, where CMake's documentation really
failed me with the find_package part). Due to differences between the versions
of various dependencies in different distributions, Fidgety may fail to compile.
Currently, Fidgety compiles *without warnings* on Arch Linux and Debian 11
(Bullseye).

# Building

Fidgety requires that you have [CMake](https://cmake.org) (>= 3.14) installed
as it is the build system used by this project. CMake version 3.14 and above
provides important functionality that this project relies on. However, only
CMake 3.18 and above have been tested.

You also need a working copy of git to download
[googletest](https://github.com/google/googletest)
from [Github](https://github.com).

You will also need to install Qt5/6, spdlog, fmt and nlohmann_json with
the corresponding *.cmake files in your system directories. This allows CMake
to search and load these packages through the `find_package` function.
Your distribution's package manager should be able to handle the installation
of these libraries for you without much trouble.

[fmt](https://github.com/fmtlib/fmt) is a critical component of Fidgety.
However, spdlog already includes fmt as a dependency, so there should
hopefully be no need to explicitly install fmt.

Preferably, you should have a recent version of Make installed so that you can
run the various common tasks (targets) in /Makefile.

The table below shows the targets defined in the Makefile and what they do.

| Target | Function |
| ------ | -------- |
| generate | Generate the build directory that CMake stores its cache in. This is also the step where dependencies are searched for and googletest is downloaded (unless specified otherwise). |
| build | Compile Fidgety and the test suite (can be opted out). |
| test | Run the test suites written for Fidgety. This will fail if the tests aren't built. |
| install | Install to the dist/ prefix. The folder can be changed by adding `DIST_DIR=<directory>` to the end of the make command. |

These targets require the use of certain coreutils (provided by GNU) like `env`
or `mkdir`. Coreutils should already be installed by your distro so you most
likely don't have to check that these programs exist.

## Supplementary Build Flags

The Makefile in the root of the project directory allows you to add
supplementary flags to configure what gets built and therefore what components
of Fidgety gets installed. You can set these supplementary flags by setting
environment variables containing a space-separated list of command line
arguments (that must be readable by CMake, CTest or CPack, depending on the
command). The contents of these environment variables are expanded at the end
of the relevant commands in the Makefile. For example, the
`CMAKE_GENERATE_SUPPFLAGS`, which contains the command line arguments for
generating the `build/` directory by CMake, is used by the Make target called
`generate`. This target calls CMake using a command that looks like so:

```shell
$(CMAKE) -B $(BUILD_DIR) -S $(SRC_DIR) $(CMAKE_GENERATE_SUPPFLAGS)
```

If `CMAKE_GENERATE_SUPPFLAGS` is defined as `-DBUILD_TESTING=OFF`, then this
command would be expanded as such:

```shell
cmake -B build/ -S . -DBUILD_TESTING=OFF
```

Other environment variables for supplementary flags exist for the other targets
in the Makefile (as specified in the table in [Building](#building)). The table
below shows what each environment variable is used for.

| Environment Variable | Function |
| -------------------- | -------- |
| CMAKE_GENERATE_SUPPFLAGS | The arguments used to generate the build the directory used by CMake. |
| CMAKE_BUILD_SUPPFLAGS | The arguments used when CMake is called to compile the various targets in Fidgety. |
| CMAKE_TEST_SUPPFLAGS | The arguments used when calling `ctest`. |
| CMAKE_INSTALL_SUPPFLAGS | The arguments used when installing (to the install prefix) the artifacts generated when building the targets in Fidgety by CMake. |

### Directory Flags

In addition, you can change the value of the variables which tell CMake where
the build directory should be generated, where build artifacts should be
installed to and even where the source files are located. Just like the
supplementary flags in [Supplementary Build Flags](#supplementary-build-flags),
you can override the default values of these variables by setting environment
variables.

| Makefile Variable (Environment Variable) | Default Value | Function |
| ---------------------------------------- | ------------- | -------- |
| DIST_DIR | dist/ | The install prefix used by the `install` target. |
| BUILD_DIR | build/ | The build directory generated by CMake. |
| SRC_DIR | ./ | The root of the project source. |

### Parallel Builds

CMake allows you to choose the maximum number of jobs (compile, linking, etc.)
happening in parallel. By default, CMake assumes that you only want one job
to take place at any one time. However, this assumption means that you cannot
take advantage of all the resources your computer has, such as a new 8-core
CPU or 128 GiB of RAM. By increasing the maximum number of jobs, you can ensure
that your hardware is fully taken advantage of, reducing the build time of
Fidgety (especially as the project as a whole grows larger with every commit).

You can set the number of jobs by adding the `--parallel <N>` flag to the
`CMAKE_BUILD_SUPPFLAGS` environment variable used by the Makefile in Fidgety's
root directory like so:

```shell
make build CMAKE_BUILD_SUPPFLAGS="--parallel 8"
```

This is akin to writing:

```shell
make -j8 -C build/
```

but the `build` target in Fidgety's root Makefile makes sure that the build
directory has already been generated first by running the `generate` target.

To choose an appropriate number for `<N>`, you can refer to
[this page on Gentoo's wiki](https://wiki.gentoo.org/wiki/MAKEOPTS).
This article recommends that `N` should be either the number of logical
processors or the amount of RAM divided by 2 GiB, whichever is lower.
The number of logical processors refers to the number of "threads" on your CPU,
you can see the number of logical processors you have by entering

```shell
nproc --all
```

in your terminal. The amount of RAM only includes the memory installed on your
computer but not the swap you have allocated from your storage disk(s). For
example, if your computer has a 6C/12T CPU (like the AMD Ryzen 5 5600X) with
16 GiB of RAM, `N` should be `8` and not `12` as the amount of RAM is the
limiting factor here.

### Building dependencies from source

By default, most of Fidgety's dependencies have been installed into the system
directories by your distro's package manager. Nonetheless, some users may want
to build the dependencies from source instead of using CMake's find_package
to look for them. There is only experimental support for this feature though.

You can change set which dependencies to build from source by setting the
following variables to `ON`:

1. FIDGETY_GTEST_FROM_SOURCE
2. FIDGETY_FMTLIB_FROM_SOURCE
3. FIDGETY_SPDLOG_FROM_SOURCE
4. FIDGETY_NLOHMANNJSON_FROM_SOURCE
5. FIDGETY_QT_FROM_SOURCE

Do note that if you intend to build `spdlog` from source, you should set
`SPDLOG_FMT_EXTERNAL` or `SPDLOG_FMT_EXTERNAL_HO` to `ON`, because the `fmt`
bundled in `spdlog` does not build successfully and hence cannot be used to
build `spdlog`.

I haven't managed to build `Qt` from source yet with my current CMakeLists.txt.
Any help would be appreciated.

### Opting out of building tests

If you do not want to conduct tests (and not have googletest pollute the dist/)
directory, you can add the following environment variable to the `make` command.

```
CMAKE_GENERATE_SUPPFLAGS="-DBUILD_TESTING=OFF"
```

For example, to build Fidgety without tests, you can enter:

```shell
make build CMAKE_GENERATE_SUPPFLAGS="-DBUILD_TESTING=OFF"
```

### Selecting Qt version

Fidgety uses Qt6 as it is the latest version of Qt. However, some distros only
ship Qt5 with no convenient way of installing Qt6 through that distro's package
manager using some workaround (I'm looking at you, Debian). For backwards
compatibility, Fidgety allows you to select which major version of Qt to use
using the variable `FIDGETY_QT_MAJOR`. This variable only accepts `5` or `6`
as a value and defaults to using Qt6 if no value was specified. Just like the in
the [building tests](#opting-out-of-building-tests) section, you can specify
which major version of Qt to use through the Makefile using this command:

```shell
make generate CMAKE_GENERATE_SUPPFLAGS="-DFIDGETY_QT_MAJOR=5"
```

