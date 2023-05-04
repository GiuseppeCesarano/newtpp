# newtpp

Newtpp is a header-only C++ library that builds on the functionality of the newt C library to help users create simple terminal user interfaces quickly and easily. The library adds type safety, utility to build UI faster, and support for RAII.

## Features

- *Type safety*: newtpp provides type-safe C++ wrappers around the newt C library functions, reducing the risk of errors caused by incorrect use of function parameters.
- *Utility*: newtpp provides a set of high-level C++ classes and functions that make it easier to build terminal UIs.
- *RAII support*: newtpp's classes use RAII (Resource Acquisition Is Initialization) to manage resources automatically, reducing the risk of resource leaks.

## Requirements

- A C++20 compiler
- The newt C library

## Getting started

To use newtpp, simply clone the repository and include the relevant header files in your C++ project:

`git clone https://github.com/GiuseppeCesarano/newtpp`

``` c++
#include "newtpp/newtpp.hpp"
```

when compiling link libnewt with `-lnewt` (clang++ or g++)

## Documentation

You can use the [examples](#examples) as a guide, or refer to the [docs](doc/doc.md) for the full class documentation.

## Examples

