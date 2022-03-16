# Building Fidgety

Fidgety requires that you have [CMake](https://cmake.org) (>= 3.14) installed
as it is the build system used by this project. CMake version 3.14 and above
provides important functionality that this project relies on. However, only
CMake 3.18 and above have been tested.

You also need a working copy of git to download
[googletest](https://github.com/google/googletest)
from [Github](https://github.com).

You will also need to install Qt5/6, Boost, spdlog, fmt and nlohmann_json with
the corresponding *.cmake files in your system directories. This allows CMake
to search and load these packages through the `find_package` function.
Your distribution's package manager should be able to handle the installation
of these libraries for you without much trouble.

[fmt](https://github.com/fmtlib/fmt) is a critical component of Fidgety.
However, spdlog already includes fmt as a dependency, so there should
hopefully be no need to explicitly install fmt.

Preferably, you should have a recent version of Make installed as Fidgety uses
Make as part of the build pipeline.

You can visit [**BUILD_MK.md**](BUILD_MK.md) to learn more about the Makefile
Fidgety uses to simplify the build process. Directly calling make was the old
way of building Fidgety, and instead you should use the `fgmk` script in the
root of the project directory.

## **`fgmk`**

`fgmk` is a python script that saves build configurations and manages the build
process for Fidgety. It's essentially a wrapper over the Makefile I wrote which
is a wrapper over CMake which is a wrapper over Make again. You can learn more
by reading [**BUILD_FGMK.md**](BUILD_FGMK.md)
