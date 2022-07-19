# Fidgety

A graphical app that edits config files using toggles, dropdowns, sliders and text entry fields.

This app got its name from those fidget toys with buttons and joysticks.

# Schematic

![Schematic](thats-how-fidgety-works.png)

# Aims

Fidgety hopes to tackle the problem of user-friendliness in Linux, where users
are usually expected to configure their system by editing config files or
issuing commands through a terminal instead of through a pleasant graphical
interface like on Windows or MacOS (especially at the system level).

In addition, Fidgety should be extensible, allowing developers to add support
for their own software so that their users can easily edit change the settings
for their apps. This can be done by allowing developers to write their own
extensions which Fidgety loads at runtime.

# Platform Availability

Fidgety will have first-class support on Linux and will be developed according
to this policy. You can try and contribute by porting this project to other
operating systems (especially on Windows, where CMake's documentation really
failed me with the find_package part). Due to differences between the versions
of various dependencies in different distributions, Fidgety may fail to compile.
Currently, Fidgety tries to target Arch Linux, Debian 11, Ubuntu 20.04 and
Fedora 35.

# Building Fidgety

See [BUILD.md](BUILD.md) for more information and [STATUS.md](STATUS.md) for
the state of development in different environments.
