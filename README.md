# 📝 MiniVim: A Raw POSIX Terminal Text Editor

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-Build-success.svg?style=flat&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey.svg)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen)

## 💡 The "Why" Behind the Project

Most early-career projects rely heavily on standard library containers and buffered I/O (`std::cin`, `std::cout`) to abstract away the operating system. I built **MiniVim** to intentionally bypass those abstractions. 

I wanted to understand what happens under the hood when a program takes total control of the hardware. This project is a deep dive into systems programming, custom data structures, memory-safe architecture, and POSIX terminal APIs. It is a fully functional text editor built completely from scratch—no third-party dependencies, just modern C++ and the operating system.

## 🚀 Core Technical Features

* **Raw POSIX Interfacing:** Bypasses canonical terminal modes using `termios` to capture exact, byte-by-byte user input (disabling echo, standard signals, and software flow control).
* **O(1) Amortized Text Editing:** Standard strings shift massive amounts of memory for simple insertions. This editor implements a custom **Gap Buffer** using raw `std::unique_ptr` memory blocks and `std::memmove`, making typing and deleting instantaneous even in 50MB+ files.
* **Double-Buffered ANSI Rendering:** Prevents the CPU lag and visual flickering common in basic terminal apps. The rendering engine constructs the entire 2D screen state in a background buffer and executes exactly *one* `write()` system call per frame.
* **Strict Memory Safety (RAII):** The terminal state is tightly managed via RAII. If the editor crashes or throws a fatal system error, the destructor guarantees the user's shell is restored to its original state, preventing terminal corruption.
* **Dynamic UI Engine:** Includes a real-time status bar tracking dynamic cursor-to-memory 2D-to-1D mapping and file persistence.

## 🧠 Architecture Overview

The codebase is strictly modularized, separating concerns to maintain clean execution:

1. **`Terminal`:** Handles OS-level hardware toggles and multi-byte ANSI escape sequence parsing (e.g., translating `\x1b[A` to the Up Arrow).
2. **`GapBuffer`:** The high-performance $O(1)$ heart of the application. Reads binary files directly into unmanaged memory blocks.
3. **`Renderer`:** The visual engine. Translates internal memory state into VT100-compatible ANSI terminal graphics.
4. **`Editor Core`:** The state machine. Manages the event-driven loop, dynamic screen scrolling math, and user intent.

## 🛠️ Getting Started

### Prerequisites
* A POSIX-compliant OS (Linux, macOS, or WSL on Windows)
* GCC or Clang (supporting C++17)
* CMake (v3.15+)

### Local Build Instructions
This project uses an out-of-source build system to keep the source tree clean and enforces strict compiler warnings (`-Wall -Wextra -pedantic`) alongside `-O3` optimizations.

```bash
# 1. Clone the repository
git clone [https://github.com/yourusername/minivim.git](https://github.com/yourusername/minivim.git)
cd minivim

# 2. Create the build directory
mkdir build && cd build

# 3. Generate and compile
cmake ..
make -j$(nproc)

# 4. Run the editor on a new or existing file
./mini_vim ../src/main.cpp
