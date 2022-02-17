# Fidgety

A graphical app that edits config files using toggles, dropdowns, sliders and text entry fields.

This app got its name from those fidget toys with buttons and joysticks.

# Schematic

![Schematic](thats-how-fidgety-works.png)

# Platform Availability

This project was created to address the UI problem in Linux where users are
usually expected to configure their system by editing config files instead of
through a simple GUI as seen on a Windows or MacOS system. Therefore, this
app will have first-class support on Linux and will be tested according to this
policy. You can try and contribute by porting this project to other operating
systems (especially on Windows, where CMake's documentation really failed me
with the find_package part). Due to differences between the versions of
various dependencies in different distributions, Fidgety may fail to compile.
Currently, Fidgety compiles *without warnings* on Arch Linux but I have yet to
test it on Debian.

# Building

Fidgety requires that you have [CMake](https://cmake.org) (>= 3.14) installed
as it is the build system used by this project. CMake version 3.14 and above
provides important functionality that this project relies on.

You also need a working copy of git to download
[googletest](https://github.com/google/googletest)
from [Github](https://github.com).

You will also need to install Qt6, spdlog and nlohmann_json with
the corresponding *.cmake files in your system directories. This allows CMake
to search and load these packages through the `find_package` function.
Your distribution's package manager should be able to handle the installation
of these libraries for you without much trouble.

Preferably, you should have a recent version of Make installed so that you can
run the various common tasks (targets) in /Makefile.

The table below shows the targets defined in the Makefile and what they do.

| Target | Function |
| ------ | -------- |
| generate | Generate the build directory that CMake stores its cache in. This is also the step where dependencies are searched for googletest is downloaded (unless specified otherwise). |
| build | Compile Fidgety and the test suite (can be opted out). |
| test | Run the test suites written for Fidgety. This will fail if the tests aren't built. |
| install | Install to the dist/ prefix. The folder can be changed by adding `DIST_DIR=<directory>` to the end of the make command. |

## Opting out of building tests

If you do not want to conduct tests (and not have googletest pollute the dist/)
directory, you can add the following environment variable to the `make` command.

```
CMAKE_GENERATE_SUPPFLAGS=-DBUILD_TESTING=OFF
```

For example, to build Fidgety without tests, you can enter:

```shell
make -j8 build CMAKE_GENERATE_SUPPFLAGS=-DBUILD_TESTING=OFF
```
