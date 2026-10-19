#include<iostream>
#include<chrono>
#include "../include/matrix.h"

int main(){
    const int SIZE=1024;
    int blockSize=64;

    std::cout<<"Initializing"<<SIZE<<"x"<<SIZE<<"matrices..."<<std::endl;

    Matrix A(SIZE*SIZE);
    Matrix B(SIZE*SIZE);
    
    fillRandom(A,SIZE);
    fillRandom(B,SIZE);

    std::cout<<"Starting Benchmark..."<<std::endl;

    auto start=std::chrono::high_resolution_clock::now();
    Matrix C1=multiplyNaive(A,B,SIZE);
    auto end=std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diffNaive=end-start;
    std::cout<<"Naive Approach:   "<<diffNaive.count()<<" s"<<std::endl;

    start=std::chrono::high_resolution_clock::now();
    Matrix C2=multiplyTransposed(A,B,SIZE);
    end=std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diffTrans=end-start;
    std::cout<<"Transpose Approach:   "<<diffTrans.count()<<" s"<<std::endl;

    start=std::chrono::high_resolution_clock::now();
    Matrix C3=multiplyTiled(A,B,SIZE,blockSize);
    end=std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diffTiled=end-start;
    std::cout<<"Tiled Approach:   "<<diffTiled.count()<<" s"<<std::endl;

    start=std::chrono::high_resolution_clock::now();
    Matrix C4=multiplyParallel(A,B,SIZE,blockSize);
    end=std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diffPar=end-start;
    std::cout<<"Parallel Approach:   "<<diffPar.count()<<" s"<<std::endl;


    std::cout<<"-----------------------------------"<<std::endl;
    std::cout<<"Speedup (Transpose): "<<diffNaive.count()/diffTrans.count()<<"x faster"<<std::endl;
    std::cout<<"Speedup (Tiled): "<<diffNaive.count()/diffTiled.count()<<"x faster"<<std::endl;
    std::cout<<"Speedup (Parallel): "<<diffNaive.count()/diffPar.count()<<"x faster"<<std::endl;

    return 0;
}