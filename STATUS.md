# Status

This document details the current state of development of Fidgety in different
environments and the problems that have been encountered. You can consult this
document to find out which distros and operating systems Fidgety is being
tested on and *maybe* find workarounds for build errors.

## Common Problems

When closing the window spawned by `fidgety`, a segfault happens.

## Arch Linux

Works fine (if you've been diligently updating your packages).

## Debian 11 (Bullseye)

Looks alright.

## Ubuntu 20.04 (Focal Fossa)

g++ can't seem to find the Qt5 header files. I suspect it's because the CMake
config files (like Qt5CoreConfig.cmake) are defective and might not be pointing
to the correct include directories. These files belong to qtbase5-dev and
hopefully Canonical uploads a fix for it soon.

## Fedora 35

Yep.
