#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pmmintrin.h>  // For SSE3 intrinsics

#ifndef __SSE3__
#error This example requires SSE3
#endif

#include "timeutil.h"

/* Size of the matrices to multiply */
#define SIZE2 14
#define SIZE (1 << SIZE2)

#define MINDEX(n, m) (((n) << SIZE2) | (m))

#define XMM_ALIGNMENT_BYTES 16

static float *mat_a __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float *vec_b __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float *vec_c __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float *vec_ref __attribute__((aligned (XMM_ALIGNMENT_BYTES)));

static void
matvec_intrinsics()
{
    /* Assume that the data size is an even multiple of the 128 bit
     * SSE vectors (i.e. 4 floats) */
    assert(!(SIZE & 0x3));
    for (int i = 0; i < SIZE; i++) {
        __m128 sum = _mm_setzero_ps();
        for (int j = 0; j < SIZE; j += 4) {
            __m128 vec_b_part = _mm_load_ps(&vec_b[j]);
            __m128 mat_a_part = _mm_load_ps(&mat_a[MINDEX(i, j)]);
            sum = _mm_add_ps(sum, _mm_mul_ps(mat_a_part, vec_b_part));
        }
        sum = _mm_hadd_ps(sum, sum);
        sum = _mm_hadd_ps(sum, sum);
        vec_c[i] = _mm_cvtss_f32(sum);
    }
}

/**
 * Reference implementation of the matvec used to verify the answer. Do NOT touch this function.
 */
static void
matvec_ref()
{
    int i, j;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            vec_ref[i] += mat_a[MINDEX(i, j)] * vec_b[j];
}

/**
 * Function used to verify the result. Do not touch this function.
 */
static int
verify_result()
{
    float e_sum;
    int i;

    e_sum = 0;
    for (i = 0; i < SIZE; i++) {
        e_sum += vec_c[i] < vec_ref[i] ?
                vec_ref[i] - vec_c[i] :
                vec_c[i] - vec_ref[i];
    }

    printf("e_sum: %.e\n", e_sum);

    return e_sum < 1E-6;
}

/**
 * Initialize mat_a and vec_b with "random" data. Write to every
 * element in mat_c to make sure that the kernel allocates physical
 * memory to every page in the matrix before we start doing
 * benchmarking.
 */
static void
init()
{
    int i, j;

    mat_a = (float *)_mm_malloc(sizeof(*mat_a) * SIZE * SIZE, XMM_ALIGNMENT_BYTES);
    vec_b = (float *)_mm_malloc(sizeof(*vec_b) * SIZE, XMM_ALIGNMENT_BYTES);
    vec_c = (float *)_mm_malloc(sizeof(*vec_c) * SIZE, XMM_ALIGNMENT_BYTES);
    vec_ref = (float *)_mm_malloc(sizeof(*vec_ref) * SIZE, XMM_ALIGNMENT_BYTES);

    if (!mat_a || !vec_b || !vec_c || !vec_ref) {
        fprintf(stderr, "Memory allocation failed\n");
        abort();
    }

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
            mat_a[MINDEX(i, j)] = ((7 * i + j) & 0x0F) * 0x1P-2F;
        vec_b[i] = ((i * 17) & 0x0F) * 0x1P-2F;
    }

    memset(vec_c, 0, sizeof(float)*SIZE);
    memset(vec_ref, 0, sizeof(float)*SIZE);
}

static void
run_multiply()
{
    struct timespec ts_start, ts_stop;
    double runtime_ref, runtime_sse;

    get_time_now(&ts_start);
    matvec_intrinsics();
    get_time_now(&ts_stop);
    runtime_sse = get_time_diff(&ts_start, &ts_stop);
    printf("Matvec using intrinsics completed in %.2f s\n", runtime_sse);

    get_time_now(&ts_start);
    matvec_ref();
    get_time_now(&ts_stop);
    runtime_ref = get_time_diff(&ts_start, &ts_stop);
    printf("Matvec reference code completed in %.2f s\n", runtime_ref);

    printf("Speedup for intrinsics: %.2f\n", runtime_ref / runtime_sse);

    if (verify_result())
        printf("Result OK\n");
    else
        printf("Result MISMATCH\n");
}

int
main(int argc, char *argv[])
{
    /* Initialize the matrices with some "random" data. */
    init();

    run_multiply();

    _mm_free(mat_a);
    _mm_free(vec_b);
    _mm_free(vec_c);
    _mm_free(vec_ref);

    return 0;
}

