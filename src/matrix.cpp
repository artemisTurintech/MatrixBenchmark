#include "../include/matrix.h"
#include<random>
#include<algorithm>
#include <thread>
#include <vector>

void fillRandom(Matrix& m,int size){
    for(int i=0; i<size*size; i++){
        m[i]=(rand()%100)/10.0;
    }
}

Matrix multiplyNaive(const Matrix& A,const Matrix& B,int size){
    Matrix C(size*size,0);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            double sum=0;
            for(int k=0; k<size; k++){
                sum+=A[i*size+k]*B[k*size+j];
            }
            C[i*size+j]=sum;
        }
    }
    return C;
}

Matrix multiplyTransposed(const Matrix& A,const Matrix& B,int size){
    Matrix C(size*size,0);
    Matrix B_T(size*size,0);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            B_T[i*size+j]=B[j*size+i];
        }
    }
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            double sum=0;
            for(int k=0; k<size; k++){
                sum+=A[i*size+k]*B_T[j*size+k];
            }
            C[i*size+j]=sum;
        }
    }
    return C;
}

Matrix multiplyTiled(const Matrix& A,const Matrix& B,int size, int blockSize){
    Matrix C(size*size,0);
    for(int ii=0; ii<size; ii+=blockSize){
        for(int kk=0; kk<size; kk+=blockSize){
            for(int jj=0; jj<size; jj+=blockSize){
                for(int i=ii; i<std::min(ii + blockSize, size); i++){
                    for(int k=kk; k<std::min(kk + blockSize, size); k++){
                        double temp=A[i*size+k];
                        for(int j=jj; j<std::min(jj + blockSize, size); j++){
                            C[i*size+j]+=temp*B[k*size+j];
                        }
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiplyParallel(const Matrix& A, const Matrix& B, int size, int blockSize){
    Matrix C(size*size, 0);

    // 1. Detect Cores
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    std::vector<std::thread> threads;
    
    // 2. Define the Worker (The "Recipe")
    auto worker = [&](int startRow, int endRow) {
        for (int ii = startRow; ii < endRow; ii += blockSize) {
            for (int kk = 0; kk < size; kk += blockSize) {
                for (int jj = 0; jj < size; jj += blockSize) {
                    for (int i = ii; i < std::min(ii + blockSize, endRow); i++) {
                        for (int k = kk; k < std::min(kk + blockSize, size); k++) {
                            double temp = A[i*size + k]; 
                            for (int j = jj; j < std::min(jj + blockSize, size); j++) {
                                C[i*size + j] += temp * B[k*size + j];
                            }
                        }
                    }
                }
            }
        }
    };

    // 3. Launch Threads (Assigning slices)
    int rowsPerThread = size / numThreads;
    for(unsigned int t = 0; t < numThreads; t++){
        int start = t * rowsPerThread;
        int end = (t == numThreads - 1) ? size : (t + 1) * rowsPerThread;
        threads.emplace_back(worker, start, end);
    }

    // 4. Wait for them
    for(auto& t : threads){
        t.join();
    }

    return C;
}