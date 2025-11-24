#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <math.h>

#ifdef _MSC_VER
#define ssize_t intptr_t
#endif

// assembly function (cdecl, 32-bit)
extern void imgCvtGrayInttoFloat(const unsigned char* in, float* out, unsigned int width, unsigned int height);

// reference C conversion
static void imgCvtGrayInttoFloat_ref(const unsigned char* in, float* out,
                                     unsigned int width, unsigned int height)
{
    size_t count = (size_t)width * (size_t)height;
    for (size_t i = 0; i < count; ++i) {
        out[i] = (float)in[i] / 255.0f;
    }
}

// Print float matrix (rows = height, cols = width)
static void print_matrix_float(const float* m, size_t width, size_t height) {
    for (size_t r = 0; r < height; ++r) {
        for (size_t c = 0; c < width; ++c) {
            printf("%.2f", m[r * width + c]);
            if (c + 1 < width) printf(" ");
        }
        printf("\n");
    }
}

int main(void) {
    size_t width = 0, height = 0;

    // Read input matrix if provided on stdin
    if (scanf("%zu %zu", &height, &width) == 2) {
        size_t count = width * height;
        unsigned char* in = (unsigned char*)malloc(count * sizeof(unsigned char));
        float* out = (float*)malloc(count * sizeof(float));
        float* out_ref = (float*)malloc(count * sizeof(float));

        if (!in || !out || !out_ref) {
            fprintf(stderr, "Allocation failed for input matrix (%zu x %zu)\n", height, width);
            free(in); free(out); free(out_ref);
            return 1;
        }

        // Read grayscale values
        for (size_t r = 0; r < height; ++r) {
            for (size_t c = 0; c < width; ++c) {
                int v = 0;
                if (scanf("%d", &v) != 1) {
                    fprintf(stderr, "Failed to read pixel at %zu,%zu\n", r, c);
                    free(in); free(out); free(out_ref);
                    return 1;
                }
                if (v < 0) v = 0;
                if (v > 255) v = 255;
                in[r * width + c] = (unsigned char)v;
            }
        }

        // Run ASM
        imgCvtGrayInttoFloat(in, out, (unsigned int)width, (unsigned int)height);

        // Run reference C version
        imgCvtGrayInttoFloat_ref(in, out_ref, (unsigned int)width, (unsigned int)height);

        // FLOAT CHECK
        int mismatch = 0;
        const float eps = 1e-6f;

        for (size_t i = 0; i < count; ++i) {
            float a = out[i];
            float b = out_ref[i];
            float diff = (a > b) ? (a - b) : (b - a);

            if (diff > eps) {
                mismatch = 1;
                break;
            }
        }

        if (mismatch) {
            printf("Mismatch detected!\nASM output:\n");
            print_matrix_float(out, width, height);

            printf("\nREF output:\n");
            print_matrix_float(out_ref, width, height);

            printf("\nCorrectness == FALSE\n\n");
        } else {
            printf("Output:\n");
            print_matrix_float(out, width, height);
            printf("\nCorrectness == TRUE\n\n");
        }

        free(in);
        free(out);
        free(out_ref);
    }

    // Timing tests (asm only)
    const int runs = 30;
    const size_t sizes[3][2] = {
        {10, 10},
        {100, 100},
        {1000, 1000}
    };

    srand((unsigned)time(NULL));

    for (int si = 0; si < 3; ++si) {
        size_t h = sizes[si][0];
        size_t w = sizes[si][1];
        size_t count = h * w;

        unsigned char* in = (unsigned char*)malloc(count * sizeof(unsigned char));
        float* out = (float*)malloc(count * sizeof(float));

        if (!in || !out) {
            printf("Allocation failed for %zux%zu — skipping timing.\n", h, w);
            free(in); free(out);
            continue;
        }

        for (size_t i = 0; i < count; ++i) {
            in[i] = (unsigned char)(rand() % 256);
        }

        // Warmup
        imgCvtGrayInttoFloat(in, out, (unsigned int)w, (unsigned int)h);

        double total_ms = 0;
        for (int r = 0; r < runs; ++r) {
            clock_t t0 = clock();
            imgCvtGrayInttoFloat(in, out, (unsigned int)w, (unsigned int)h);
            clock_t t1 = clock();
            total_ms += (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
        }

        printf("%zux%zu avg = %.6f ms\n", h, w, total_ms / runs);

        free(in);
        free(out);
    }

    return 0;
}
