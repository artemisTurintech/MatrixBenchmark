# Commands

Run all commands from the repository root. Each is a single line with no shell operators, so
it works unchanged in PowerShell (including Windows PowerShell 5.1), cmd, Git Bash, and on the
Artemis runner.

Test and Benchmark run whatever is in `build/`, so run Compile first after any source change
to make sure they reflect the repository as it is now.

**Prerequisites:** CMake ≥ 3.25 (for workflow presets), Ninja, and a C++17 `g++`.

## Compile

```bash
cmake --workflow --preset default --fresh
```

Configures and builds in one command using the `default` workflow in `CMakePresets.json`
(Ninja, `g++`, output in `build/`), stopping with a non-zero exit if either step fails.
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

It also writes `artemis_results.json` in the repository root with the timings in
milliseconds (lower is better), which is where Artemis reads its metrics from:

```json
{"naive_ms": 2878.5, "transpose_ms": 467.1, "tiled_ms": 134.0, "parallel_ms": 60.1}
```

Any old results file is deleted when the benchmark starts, so a failed run never reports
stale numbers.

`plot_results.py` does **not** run the benchmark. Its timings are typed into the script, so
update `times` by hand before regenerating `benchmark_plot.png`.
