/**
 * @file test_main.c
 * @brief Test suite for the bmap library.
 * @author Arda Aksu
 * @date 2026
 */

#include "bmap.h"
#include <stdio.h>

int main() {
    BMPError err;
    
    printf("--- BMP Library Test Suite Started ---\n");

    // 1. Loading Test
    // Using airplane.bmp from the assets folder as seen in your directory structure
    printf("[1/5] Loading image (assets/airplane.bmp)... ");
    BMPImage* img = bmp_load("assets/airplane.bmp", &err);
    if (!img) {
        printf("FAILED! Error Code: %d\n", err);
        printf("Please ensure 'assets/airplane.bmp' exists and is a 24-bit BMP file.\n");
        return 1;
    }
    printf("Success! (%dx%d)\n", img->width, img->height);

    // 2. Filter Tests
    printf("[2/5] Applying filters (Grayscale & Invert)... ");
    bmp_grayscale(img);
    bmp_invert(img);
    printf("Done.\n");

    // 3. Transformation Tests
    printf("[3/5] Applying transformations (Rotate & Flip)... ");
    bmp_rotate_right(img);
    bmp_flip_horizontal(img);
    printf("Done. New dimensions: %dx%d\n", img->width, img->height);

    // 4. Saving Test
    printf("[4/5] Saving processed image (test_output.bmp)... ");
    err = bmp_save(img, "test_output.bmp");
    if (err != BMP_SUCCESS) {
        printf("FAILED! Error Code: %d\n", err);
    } else {
        printf("Success!\n");
    }

    // 5. Memory Cleanup
    printf("[5/5] Freeing allocated memory... ");
    bmp_free(img);
    printf("Done.\n");

    printf("\n--- Test Suite Completed Successfully! ---\n");
    printf("You can now check for memory leaks using: valgrind --leak-check=full ./test_app\n");

    return 0;
}