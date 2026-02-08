/**
 * @file bmap.h
 * @brief A modular and lightweight BMP image processing library in C.
 * @author Arda Aksu
 * @date 2026
 */

#ifndef BMAP_H
#define BMAP_H

#include <stdint.h>

/* ========================================================================= *
 * DATA TYPES                                 *
 * ========================================================================= */

/**
 * @brief Enum for error handling and status reporting.
 */
typedef enum {
    BMP_SUCCESS = 0,               /**< Operation completed successfully */
    BMP_ERR_FILE_NOT_FOUND = 1,    /**< File could not be opened or found */
    BMP_ERR_INVALID_FORMAT = 2,    /**< File is not a valid BMP or unsupported depth */
    BMP_ERR_MALLOC_FAILED = 3      /**< Memory allocation failed (RAM is full) */
} BMPError;

#pragma pack(push, 1)
/**
 * @brief Structure representing a single RGB pixel.
 * BMP format stores pixels in BGR (Blue-Green-Red) order.
 */
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;
#pragma pack(pop)

/**
 * @brief Main structure representing an image loaded in memory.
 * Users interact primarily with this structure.
 */
typedef struct {
    int width;      /**< Image width in pixels */
    int height;     /**< Image height in pixels */
    Pixel* data;    /**< Flat array of pixels (row-major order) */
} BMPImage;


/* ========================================================================= *
 * CORE FUNCTIONS                                *
 * ========================================================================= */
/**
 * @brief Loads a BMP file from disk into memory.
 * @param filename Path to the BMP file.
 * @param err_out Pointer to store error status (can be NULL).
 * @return Pointer to loaded BMPImage, or NULL on failure.
 */
BMPImage* bmp_load(const char* filename, BMPError* err_out);

/**
 * @brief Saves the BMPImage from memory to a file on disk.
 * Handles row padding automatically.
 * @param image Pointer to the image structure.
 * @param filename Target file path.
 * @return BMP_SUCCESS on success, or error code on failure.
 */
BMPError bmp_save(const BMPImage* image, const char* filename);

/**
 * @brief Frees the memory allocated for the image and its pixel data.
 * @param image Pointer to the image structure to be destroyed.
 */
void bmp_free(BMPImage* image);


/* ========================================================================= *
 * PIXEL ACCESS METHODS                             *
 * ========================================================================= */

/**
 * @brief Retrieves the pixel color at coordinates (x, y).
 * Performs boundary checks to prevent memory errors.
 */
Pixel bmp_get_pixel(const BMPImage* image, int x, int y);

/**
 * @brief Updates the pixel color at coordinates (x, y).
 * Performs boundary checks to prevent memory errors.
 */
void bmp_set_pixel(BMPImage* image, int x, int y, Pixel color);


/* ========================================================================= *
 * IMAGE TRANSFORMATIONS                            *
 * ========================================================================= */

/**
 * @brief Rotates the image 90 degrees clockwise.
 */
void bmp_rotate_right(BMPImage* image);

/**
 * @brief Flips the image horizontally (Mirror effect).
 */
void bmp_flip_horizontal(BMPImage* image);


/* ========================================================================= *
 * FILTERS                                   *
 * ========================================================================= */

/**
 * @brief Converts the image to grayscale.
 */
void bmp_grayscale(BMPImage* image);

/**
 * @brief Inverts the colors of the image (Negative effect).
 */
void bmp_invert(BMPImage* image);

#endif // BMAP_H