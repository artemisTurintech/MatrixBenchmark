# Commands

Run all commands from the repository root. They work in both Git Bash and PowerShell.

Test and Benchmark run whatever is in `build/`, so run Compile first after any source change
to make sure they reflect the repository as it is now.

**Prerequisites:** CMake ≥ 3.24 (for `--fresh`), Ninja, and a C++17 `g++`.

## Compile

```bash
cmake --fresh -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=g++
cmake --build build
```

`--fresh` discards any existing `build/CMakeCache.txt`, so a cache left over from another
checkout or toolchain can't leak in. Optimization flags (`-O3 -march=native`) come from
`CMakeLists.txt`.

## Test

```bash
ctest --test-dir build --output-on-failure
```

Runs the `matrix_tests` target (`tests/test_matrix.cpp`).

## Benchmark

```bash
./build/benchmark
```

Multiplies two 1024×1024 matrices with each method (Naive, Transpose, Tiled, Parallel) and
prints the time for each and its speedup over Naive.

`plot_results.py` does **not** run the benchmark. Its timings are typed into the script, so
update `times` by hand before regenerating `benchmark_plot.png`.
