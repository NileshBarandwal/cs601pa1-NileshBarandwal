#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <immintrin.h>
#include <omp.h>

#ifdef SINGLE
typedef float matrix_type;
#else
typedef double matrix_type;
#endif

#ifdef STACKALLOCATED
#define INPUTSIZE 256
#endif

void matrix_multiply(const matrix_type* A, const matrix_type* B, matrix_type* C, int n) {
#ifdef ORDER_IJK
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix_type sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
#endif

#ifdef ORDER_IKJ
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            matrix_type a = A[i * n + k];
            for (int j = 0; j < n; ++j) {
                C[i * n + j] += a * B[k * n + j];
            }
        }
    }
#endif

#ifdef ORDER_JIK
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            matrix_type sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
#endif

#ifdef ORDER_JKI
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < n; ++k) {
            matrix_type b = B[k * n + j];
            for (int i = 0; i < n; ++i) {
                C[i * n + j] += A[i * n + k] * b;
            }
        }
    }
#endif

#ifdef ORDER_KIJ
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            matrix_type a = A[i * n + k];
            for (int j = 0; j < n; ++j) {
                C[i * n + j] += a * B[k * n + j];
            }
        }
    }
#endif

#ifdef ORDER_KJI
    #ifdef PARALLEL
    #pragma omp parallel for
    #endif
    for (int k = 0; k < n; ++k) {
        for (int j = 0; j < n; ++j) {
            matrix_type b = B[k * n + j];
            for (int i = 0; i < n; ++i) {
                C[i * n + j] += A[i * n + k] * b;
            }
        }
    }
#endif
}

int main(int argc, char* argv[]) {
    int n;
#ifdef STACKALLOCATED
    n = INPUTSIZE;
    matrix_type A[INPUTSIZE][INPUTSIZE], B[INPUTSIZE][INPUTSIZE], C[INPUTSIZE][INPUTSIZE];
#else
    n = atoi(argv[1]);
    matrix_type* A = new matrix_type[n * n];
    matrix_type* B = new matrix_type[n * n];
    matrix_type* C = new matrix_type[n * n];
#endif

    std::srand(std::time(NULL));
#ifdef STACKALLOCATED
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            B[i][j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            C[i][j] = 0;
        }
    }
#else
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            B[i * n + j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            C[i * n + j] = 0;
        }
    }
#endif

    const auto start = std::chrono::steady_clock::now();
    matrix_multiply(A, B, C, n);
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<float> elapsedtime = end - start;

    std::cout << "elapsed seconds: " << elapsedtime.count() << std::endl;

#ifdef STACKALLOCATED
    // Output for validation
    std::cout << "Matrix C (result):" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }
#endif

#ifndef STACKALLOCATED
    delete[] A;
    delete[] B;
    delete[] C;
#endif

    return 0;
}