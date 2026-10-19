#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include "../include/matrix.h"

static int failures = 0;

// Deterministic fill so every run compares the same data.
static void fillDeterministic(Matrix& m, int size, unsigned seed) {
    unsigned state = seed;
    for (int i = 0; i < size * size; i++) {
        state = state * 1664525u + 1013904223u;
        m[i] = ((state >> 16) % 100) / 10.0;
    }
}

// Straightforward reference product, independent of the optimized versions.
static Matrix reference(const Matrix& A, const Matrix& B, int size) {
    Matrix C(size * size, 0.0);
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            double temp = A[i * size + k];
            for (int j = 0; j < size; j++) {
                C[i * size + j] += temp * B[k * size + j];
            }
        }
    }
    return C;
}

static void expectEqual(const std::string& name, const Matrix& got, const Matrix& want) {
    if (got.size() != want.size()) {
        std::printf("FAIL %-40s size %zu != %zu\n", name.c_str(), got.size(), want.size());
        failures++;
        return;
    }
    for (size_t i = 0; i < want.size(); i++) {
        double tol = 1e-9 * (1.0 + std::fabs(want[i]));
        if (!(std::fabs(got[i] - want[i]) <= tol)) {
            std::printf("FAIL %-40s index %zu: %.12g != %.12g\n", name.c_str(), i, got[i], want[i]);
            failures++;
            return;
        }
    }
    std::printf("ok   %s\n", name.c_str());
}

static void runCase(int size, int blockSize) {
    Matrix A(size * size), B(size * size);
    fillDeterministic(A, size, 12345u);
    fillDeterministic(B, size, 67890u);

    const Matrix want = reference(A, B, size);
    const std::string suffix = " (size=" + std::to_string(size) + ", block=" + std::to_string(blockSize) + ")";

    expectEqual("multiplyNaive" + suffix, multiplyNaive(A, B, size), want);
    expectEqual("multiplyTransposed" + suffix, multiplyTransposed(A, B, size), want);
    expectEqual("multiplyTiled" + suffix, multiplyTiled(A, B, size, blockSize), want);
    expectEqual("multiplyParallel" + suffix, multiplyParallel(A, B, size, blockSize), want);
}

int main() {
    runCase(64, 64);    // matches the benchmark's block size
    runCase(128, 64);
    runCase(200, 64);   // size not a multiple of the block size
    runCase(256, 32);

    // fillRandom must touch every element of the buffer.
    {
        const int size = 32;
        Matrix m(size * size, -1.0);
        fillRandom(m, size);
        bool untouched = false;
        for (double v : m) untouched = untouched || (v < 0.0);
        if (untouched) {
            std::printf("FAIL fillRandom left elements uninitialized\n");
            failures++;
        } else {
            std::printf("ok   fillRandom fills the whole matrix\n");
        }
    }

    if (failures > 0) {
        std::printf("\n%d check(s) FAILED\n", failures);
        return 1;
    }
    std::printf("\nAll checks passed\n");
    return 0;
}
