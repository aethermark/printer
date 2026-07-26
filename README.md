# Printer

> A debug printer for c++.

---

## Overview

This project is part of a larger effort to build the modern software stack behind scientific document creation from first principles.

The goal is not simply to recreate existing software, but to understand the algorithms, data structures, and design decisions behind the tools that are often taken for granted.

Each repository in this ecosystem is developed as an independent, reusable C++ library with minimal dependencies.

---

## Goals

- Learn and implement the underlying algorithms from scratch.
- Keep the public API small and well documented.
- Design reusable libraries rather than standalone applications.
- Prefer clarity and correctness over optimization.
- Document implementation decisions throughout development.

---

## Project Status

| Feature       | Status |
| ------------- | :----: |
| Core Library  |   🚧   |
| Examples      |   🚧   |
| Tests         |   🚧   |
| Documentation |   🚧   |
| Benchmarks    |   ⬜   |

Legend:

- ⬜ Planned
- 🚧 In Progress
- ✅ Complete

---

## Building

### Clone

```bash
git clone https://github.com/aethermark/printer.git
cd printer
```

### Configure

```bash
make debug
```

### Run playground

```bash
make run
```

all other commands can be found in the Makefile.

---

## Playground

During development, experiments are performed inside the playground executable.
The playground links against the library exactly like an external application would.

---

## Using as a Dependency

This project is designed to be included as a Git submodule.

```text
third_party/
└── printer/
```

In CMake:

```cmake
add_subdirectory(third_party/printer)

target_link_libraries(my_application
    PRIVATE
        printer
)
```

---

## Documentation

Additional implementation notes can be found in the `docs/` directory.
These documents focus on the reasoning behind the implementation rather than only describing the API.

---

## Design Principles

- Modern C++ (C++26)
- Cross-platform
- Minimal dependencies
- Small public API
- Test-driven where practical
- Clear separation between interface and implementation

---

## License

This project is licensed under the MIT License.

See the `LICENSE` file for details.
