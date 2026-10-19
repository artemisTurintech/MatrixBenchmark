#pragma once
#include<vector>

using Matrix=std::vector<double>;

void fillRandom(Matrix& m, int size);

Matrix multiplyNaive(const Matrix& A,const Matrix& B, int size);

Matrix multiplyTransposed(const Matrix& A,const Matrix& B, int size);

Matrix multiplyTiled(const Matrix& A,const Matrix& B, int size, int blockSize);

Matrix multiplyParallel(const Matrix& A, const Matrix& B, int size, int blockSize);


