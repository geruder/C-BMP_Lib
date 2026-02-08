/**
 * @file bmap.c
 * @brief Implementation of the BMP image processing library functions.
 * * Contains the logic for file I/O operations (reading/writing BMP headers),
 * memory management for pixel data, and image transformation algorithms 
 * like 90-degree rotation and grayscale filtering.
 * @author Arda Aksu
 * @date 2026
 * @see bmap.h for function prototypes and error definitions.
 */

#include "bmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BINARY_READ "rb"
#define BINARY_WRITE "wb"

#pragma pack(push, 1)
typedef struct {
    uint16_t type;              
    uint32_t size;              
    uint16_t reserved1, reserved2;
    uint32_t offset;            
} BMPFileHeader;

typedef struct {
    uint32_t size;              
    int32_t  width;
    int32_t  height;
    uint16_t planes;            
    uint16_t bit_count;         
    uint32_t compression;       
    uint32_t size_image;
    int32_t  x_pixels_per_meter;
    int32_t  y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
} BMPInfoHeader;
#pragma pack(pop)

static int calculate_padding(int width) {
    return (4 - (width * sizeof(Pixel)) % 4) % 4;
}

/* --- Save and Load Methods --- */

BMPImage* bmp_load(const char* filename, BMPError* err_out){
    FILE *filepath = fopen(filename, BINARY_READ);
    if(!filepath) {
        if(err_out) *err_out = BMP_ERR_FILE_NOT_FOUND;
    }
    
    BMPFileHeader fh;
    BMPInfoHeader ih;

    fread(&fh, sizeof(BMPFileHeader), 1, filepath);
    fread(&ih, sizeof(BMPInfoHeader), 1, filepath);

    if(fh.type != 0x4D42 || ih.bit_count != 24) {
        if(err_out) *err_out = BMP_ERR_INVALID_FORMAT;
        fclose(filepath);
        return NULL;
    }

    BMPImage* img = (BMPImage*)malloc(sizeof(BMPImage));
    img -> height = abs(ih.height);
    img -> width  = ih.width;
    img -> data = (Pixel*)malloc(img->height * img->width * sizeof(Pixel));

    if(!img->data) {
        if(err_out) *err_out = BMP_ERR_MALLOC_FAILED;
        free(img);
        fclose(filepath);
        return NULL;
    }

    int padding = calculate_padding(img->width);
    fseek(filepath, fh.offset, SEEK_SET);

    for(int i = 0; i < img->height; i++) {
        fread(&img->data[i * img->width], sizeof(Pixel), img->width, filepath);
        fseek(filepath, padding, SEEK_CUR);
    }
    
    fclose(filepath);
    if(err_out) *err_out = BMP_SUCCESS;
    return img;
}

BMPError bmp_save(const BMPImage* image, const char* filename) {
    FILE* filepath = fopen(filename, BINARY_WRITE);
    if(!filepath) return BMP_ERR_FILE_NOT_FOUND;

    int padding = calculate_padding(image->width);
    uint32_t image_size = (image->width * sizeof(Pixel) + padding) * image->height;

    BMPFileHeader fh = {0x4D42, sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + image_size, 0, 0, 54};
    BMPInfoHeader ih = {40, image->width, image->height, 1, 24, 0, image_size, 2835, 2835, 0, 0};

    fwrite(&fh, sizeof(BMPFileHeader), 1, filepath);
    fwrite(&ih, sizeof(BMPInfoHeader), 1, filepath);

    uint8_t padding_bytes[3] = {0, 0, 0};
    for (int i = 0; i < image->height; i++) {
        fwrite(&image->data[i * image->width], sizeof(Pixel), image->width, filepath);
        fwrite(padding_bytes, 1, padding, filepath);
    }

    fclose(filepath);
    return BMP_SUCCESS;
}

void bmp_free(BMPImage* image) {
    if (image) {
        if (image->data) free(image->data);
        free(image);
    }
}

/* --- Pixel Access Methods --- */

Pixel bmp_get_pixel(const BMPImage* image, int x, int y) {
    if (image == NULL || image->data == NULL || 
        x < 0 || x >= image->width || 
        y < 0 || y >= image->height) {

        Pixel black = {0, 0, 0};
        return black;
    }

    int index = (y * image->width) + x;
    return image->data[index];
}

void bmp_set_pixel(BMPImage* image, int x, int y, Pixel color) {
    if (image == NULL || image->data == NULL || 
        x < 0 || x >= image->width || 
        y < 0 || y >= image->height) {
        return; 
    }

    int index = (y * image->width) + x;
    image->data[index] = color;
}

/* --- Image Rotations --- */

void bmp_rotate_right(BMPImage* image) {
    if (image == NULL || image->data == NULL) {
        return;
    }

    int new_height = image -> width;
    int new_width = image -> height;

    Pixel* new_data = (Pixel*)malloc(new_width * new_height * sizeof(Pixel));
    if (!new_data) return; 
    
    for(int i = 0; i < image->height; i++) {
        for(int j = 0; j < image->width; j++) {
            int new_index = (j * new_width) + (image->height - 1 - i);
            int old_index = (i * image->width) + j;
            
            new_data[new_index] = image->data[old_index];
        }
    }

    free(image->data);
    image->data = new_data;
    image->width = new_width;
    image->height = new_height;
}

void bmp_flip_horizontal(BMPImage* image) {
    if (!image || !image->data) return;

    Pixel* new_data = (Pixel*)malloc(image->width * image->height * sizeof(Pixel));
    if(!new_data) return;

    for(int i = 0; i < image->height; i++) {
        for(int j = 0; j < image->width; j++) {
            int new_index = (i * image->width) + (image->width - 1 - j);
            int old_index = (i * image->width) + j;

            new_data[new_index] = image->data[old_index];
        }
    }
    free(image->data);
    image->data = new_data;
}

/* --- Image Fılters --- */

void bmp_grayscale(BMPImage* image) {
    if (!image || !image->data) return;

    for(int i = 0; i < image->height * image->width; i++) {
        uint8_t avg = (image->data[i].red + image->data[i].green + image->data[i].blue) / 3;
        
        image->data[i].red = avg;
        image->data[i].green = avg;
        image->data[i].blue = avg;
    }
}


void bmp_invert(BMPImage* image) {
    if (!image || !image->data) return;

    for(int i = 0; i < image->height; i++) {
        for(int j = 0; j < image->width; j++) {
            int index = (i * image->width) + j;

            image->data[index].blue= (255 - image->data[index].blue);
            image->data[index].red= (255 - image->data[index].red);
            image->data[index].green= (255 - image->data[index].green);
        }
    }
}