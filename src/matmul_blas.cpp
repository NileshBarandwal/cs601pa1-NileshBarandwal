#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cblas.h>

#ifdef SINGLE
typedef float matrix_type;
#define cblas_dot cblas_sdot
#define cblas_gemm cblas_sgemm
#define MATRIX_TYPE float
#else
typedef double matrix_type;
#define cblas_dot cblas_ddot
#define cblas_gemm cblas_dgemm
#define MATRIX_TYPE double
#endif

// Function to perform matrix multiplication using BLAS's dot product
void matmul_dot_product(const matrix_type* A, const matrix_type* B, matrix_type* C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i * n + j] = cblas_dot(n, A + i * n, 1, B + j * n, 1); // Ensure correct strides
        }
    }
}

// Function to perform matrix multiplication using BLAS's gemm
void matmul_gemm(const matrix_type* A, const matrix_type* B, matrix_type* C, int n) {
    matrix_type alpha = 1.0;
    matrix_type beta = 0.0;
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, alpha, A, n, B, n, beta, C, n);
}

int main(int argc, char* argv[]) {
    int n;
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <matrix_size>" << std::endl;
        return 1;
    }
    n = std::atoi(argv[1]);
    if (n <= 0) {
        std::cerr << "Invalid matrix size." << std::endl;
        return 1;
    }

    matrix_type* A = new matrix_type[n * n];
    matrix_type* B = new matrix_type[n * n];
    matrix_type* C = new matrix_type[n * n];

    std::srand(std::time(NULL));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            B[i * n + j] = std::rand() / static_cast<matrix_type>(RAND_MAX);
            C[i * n + j] = 0;
        }
    }

    // Measure time for dot product method
    const auto start_dot = std::chrono::steady_clock::now();
    matmul_dot_product(A, B, C, n);
    const auto end_dot = std::chrono::steady_clock::now();
    const std::chrono::duration<float> elapsedtime_dot = end_dot - start_dot;
    std::cout << "Dot Product elapsed seconds: " << elapsedtime_dot.count() << std::endl;

    // Measure time for gemm method
    const auto start_gemm = std::chrono::steady_clock::now();
    matmul_gemm(A, B, C, n);
    const auto end_gemm = std::chrono::steady_clock::now();
    const std::chrono::duration<float> elapsedtime_gemm = end_gemm - start_gemm;
    std::cout << "GEMM elapsed seconds: " << elapsedtime_gemm.count() << std::endl;

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}

