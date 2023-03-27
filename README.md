# Vortex Renderer is a cross platform renderer written in C++ and supporting multiple graphics API

The requirements are:

- CMake 3.11 or better; 3.14+ highly recommended.
- A C++17 compatible compiler
- Git (highly recommend)
- cppcheck (optional)
- include-what-you-need (optional)
- Doxygen (optional)

To configure:

```bash
cmake -S . -B build
```

Add `-G{Generator}` where {Generator} is your desired generator.

To build:

```bash
cmake --build build
```

To build docs (requires Doxygen, output in `build/docs/html`):

```bash
cmake --build build --target docs
```

To use an IDE, such as Xcode or Visual Studio (where {IDE} is your desired IDE):

```bash
cmake -S . -B build -G{IDE}
cmake --open build
```

To use cpplint and/or include-what-you-need

```bash
cmake -DCMAKE_CXX_CPPCHECK:FILEPATH=cppcheck -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use -S . -B build
cmake --open build
```

Style guidelines:

- See .clang-format file
