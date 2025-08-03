# inknit

**inknit** is a fast and flexible 2D graphics library for embedded systems, written in portable C.  
It is designed to run efficiently across a wide range of bit depths—from 1bpp to 4bpp—while
remaining platform-agnostic and testable on desktop environments.

The name **inknit** comes from the idea of "knitting pixels with ink"—a metaphor 
for composing pixels with precision and care, especially on resource-constrained systems.


## ✨ Features

- 🧵 **Bit-depth aware rendering**: Efficient support for 1bpp, 2bpp, and 4bpp 
  framebuffers
- 🚀 **Performance-focused**: Optimized for speed with bitwise operations and minimal abstraction  
- 🖼️ **Essential drawing primitives**: `point`, `hline`, `vline`, `line`, `rect`, `circle`,
  and `blit`
- 🔍 **Image scaling**: Includes nearest-neighbor and bilinear interpolation
- 🧽 **Framebuffer clearing**: Fast clearing for arbitrary bit depths
- 🔧 **No platform dependencies**: Not tied to any SDK or hardware
- 🧪 **PC-friendly testing**: Designed to be testable in desktop environments
- ⚖️ **GPLv3 licensed**: Free and open-source under a strong copyleft license


## 📆 Planned Features

- Filled shapes (e.g. filled rectangles and circles)
- Clipping regions and viewports
- Font rendering helpers


## 📦 Requirements

### 🧠 Runtime Requirements

- **Memory**: Designed for systems with limited SRAM. For best performance with features like scaling, ≥32KB SRAM is recommended.
- **Framebuffer**: You must provide your own framebuffer implementation. inknit works with packed-pixel formats (1bpp, 2bpp, 4bpp).
- **OS Independence**: No operating system is required. inknit is fully freestanding and RTOS-agnostic.

### 🧰 Build Requirements

To build and use **inknit**, the following tools are required:

- **CMake** (version 3.21 or later)
- **ninja-build** (used by CMake for fast builds)
- **C compiler** (GCC or Clang)
- **C++17-capable compiler** (required for tests and benchmarks)
- **Standard POSIX environment** (Linux or WSL 2 recommended)

Even though the core library is written in C, the unit tests and benchmarks are implemented in
modern C++.

### 🔧 Installing Dependencies on Debian/Ubuntu

For general use on an AMD64 (x86-64) host:

```bash
sudo apt update
sudo apt install build-essential cmake git g++ ninja-build
```

This provides everything you need to build the library and run tests and benchmarks.

### 🚀 Cross-Compiling and Debugging Cross-Built Binaries

To build for other architectures and debug those builds, install the appropriate cross-toolchains
and user-mode emulators:

#### Cross-Toolchains

- **AArch64 (ARM64)**:

  ```bash
  sudo apt install g++-aarch64-linux-gnu
  ```

- **x86 (32-bit)**:

  ```bash
  sudo apt install g++-i686-linux-gnu
  ```

- **Clang-based build** (optional):

  ```bash
  sudo apt install clang
  ```

#### Running and Debugging Cross-Binaries

To run and debug cross-built binaries (e.g., ARM64 on AMD64), install GNU debugger enabled all
architectures and QEMU user emulation:

```bash
sudo apt install gdb-multiarch qemu-user
```

Note: This is not required for native or x86 (32-bit on AMD64) builds.

### 🧩 For RP2040/RP2350 (Raspberry Pi Pico/2)

- On **Ubuntu**, use:

  ```bash
  sudo apt install g++-arm-none-eabi libstdc++-arm-none-eabi-newlib
  ```

- On **Debian**, the default `g++-arm-none-eabi` is often **too old** to support the Pico C SDK.
  Consider installing a newer version manually from
  [Arm’s official GNU Toolchain](https://developer.arm.com/downloads/-/gnu-rm), or using a
  container or Nix-based environment.

### ⚙️ CMake Options

| Option                     | Type     | Default      | Description                                                                                                                |
|----------------------------|----------|--------------|----------------------------------------------------------------------------------------------------------------------------|
| `INKNIT_DISABLE_LUT`       | `BOOL`   | `OFF`        | Disable internal lookup tables for drawing operations. Disabling may reduce binary size but lower performance.             |
| `INKNIT_ENABLE_BENCHMARKS` | `BOOL`   | `ON`         | Enable benchmarking utilities. Exposes internal functions and builds a benchmark suite (requires C++17).                   |
| `INKNIT_ENABLE_UNROLL`     | `BOOL`   | `OFF`        | Enable loop unrolling in selected rendering routines for better performance on some targets.                               |
| `INKNIT_USE_MEMORY_H`      | `BOOL`   | `ON`         | Use `memory.h` instead of primitive operations for `memcpy`/`memset` in certain embedded environments.                     |
| `INKNIT_TARGET`            | `STRING` | `""` (empty) | Target platform hint. Set to `"pico"` when building for RP2040 / RP2350 via `CMakePresets.json`. Defaults to empty string. |

Example:

```sh
cmake -B build -DINKNIT_DISABLE_LUT=ON -DINKNIT_USE_MEMORY_H=ON
```


## 🛠️ Build Instructions

### 📁 Standalone Build

You can build inknit and its test/benchmark tools using CMake:

```sh
git clone https://github.com/mntone/inknit.git
cd inknit
cmake -B build
cmake --build build
```

This builds:

- The core C library (`inknit`) as a static library
- Optional C++ test and benchmark tools (requires C++17)

### 🧩 Integrating with your own project (as a subdirectory)

If you are using CMake in your own embedded project:

```cmake
add_subdirectory(external/inknit)
target_link_libraries(my_app PRIVATE inknit)
target_include_directories(my_app PRIVATE external/inknit/inc)
```

> 📌 `inknit` does not depend on any platform SDK or OS. You must provide your own framebuffer output mechanism.


## 👥 Contributing

Contributions are welcome!

- Issues and feature requests are appreciated
- Pull requests should follow the style of the existing codebase
- Please use C17 (or later) for C code and C++17 for benchmarks/tests

If you’re adding support for a new platform or framebuffer format, feel free to open
a discussion first.


## 📜 License

This project is licensed under the terms of the GNU General Public License v3.0 (GPLv3).
See the [`LICENSE`](//github.com/mntone/inknit/blob/main/LICENSE) file for full details.

You are free to use, modify, and distribute this software under the terms of the GPL,
provided that any derivative work is also distributed under the same license.


## 👤 Author

inknit is developed and maintained by *mntone*.

- GitHub: https://github.com/mntone
- Mastodon: https://mstdn.jp/@mntone
- X: https://x.com/mntone

---

`inknit` aims to be your go-to framebuffer rendering engine for embedded projects that demand both **performance and flexibility**—whether you're working with monochrome E Ink displays, grayscale OLEDs, or high-color LCD panels.
