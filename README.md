libuiohook
==========

A multi-platform C library to provide global input/ouput hooking from userland.

## Compiling
Prerequisites: pkg-config, libtool, gcc, clang or msys2/mingw32

### With autotools
Other prerequisites: autotools

    ./bootstrap.sh
    ./configure
    make && make install


### With CMake
Other prerequisites: cmake

From the desired build directory:

    cmake /path/to/the/source/directory
    make

## Useage
* [Hook Demo](https://github.com/kwhat/libuiohook/blob/master/src/demo_hook.c)
* [Async Hook Demo](https://github.com/kwhat/libuiohook/blob/master/src/demo_hook_async.c)
* [Event Post Demo](https://github.com/kwhat/libuiohook/blob/master/src/demo_post.c)
* [Properties Demo](https://github.com/kwhat/libuiohook/blob/master/src/demo_properties.c)
* [Public Interface](https://github.com/kwhat/libuiohook/blob/master/include/uiohook.h)
* Please see the man pages for function documentation.
