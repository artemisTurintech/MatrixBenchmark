#include<iostream>
#include<cstdio>
#include<chrono>
#include "../include/matrix.h"

int main(){
    const int SIZE=1024;
    int blockSize=64;

    // Remove any previous results so a failed run can't report stale metrics.
    std::remove("artemis_results.json");

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

    // Metrics for Artemis, in milliseconds (lower is better). Written to a temp
    // file and renamed so a partial write is never picked up.
    std::FILE* out=std::fopen("artemis_results.json.tmp","w");
    if(!out){
        std::cerr<<"Failed to write artemis_results.json"<<std::endl;
        return 1;
    }
    int written=std::fprintf(out,"{\"naive_ms\": %.6f, \"transpose_ms\": %.6f, \"tiled_ms\": %.6f, \"parallel_ms\": %.6f}\n",
        diffNaive.count()*1000.0,diffTrans.count()*1000.0,diffTiled.count()*1000.0,diffPar.count()*1000.0);
    if(std::fclose(out)!=0 || written<0 ||
       std::rename("artemis_results.json.tmp","artemis_results.json")!=0){
        std::cerr<<"Failed to write artemis_results.json"<<std::endl;
        return 1;
    }

    return 0;
}