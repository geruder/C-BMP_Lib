# BMP Image Processing Library (C-API)

A high-performance, modular C library for 24-bit BMP image manipulation. This project is designed as a standalone API, perfect for embedded systems development and software engineering portfolios.

## 🚀 Key Features
- **Core Operations:** Robust loading/saving of 24-bit BMP files.
- **Image Filters:** Fast Grayscale and Color Inversion algorithms.
- **Transformations:** 90° Clockwise Rotation and Horizontal Flipping.
- **Safety:** Built-in error handling and zero-memory-leak architecture.

## 📁 Project Structure
- `include/`: Contains `bmap.h` (API interface).
- `src/`: Library implementation (`bmap.c`).
- `assets/`: Sample images and visual test data.
- `test_main.c`: Example application using the API.

## 🛠️ Build & Installation
The library uses a cross-platform Makefile. Depending on your system environment, use the appropriate command:

### 1. Build the Static Library (.a)
This command generates `libbmap.a`, which is the actual API package you will distribute.

```bash
mingw32-make
#or
make
```

### 2. Run the Test Suite
To verify the filters and transformations work correctly:

```bash
mingw32-make test
# or simply 'make test'
```

## 📖 API Integration Guide
To integrate this library into your own project:

1. **Include:** Add `bmap.h` to your source code.
2. **Link:** Compile your application by linking the static library.

### Compilation Command
```bash
gcc main.c -L. -lbmap -Iinclude -o my_app
```

## 💻 Quick Code Example

```c
#include "bmap.h"

int main() {
    BMPError err;
    BMPImage* img = bmp_load("assets/airplane.bmp", &err);
    
    if (img) {
        bmp_grayscale(img);      // Apply grayscale filter
        bmp_rotate_right(img);   // Rotate 90 degrees
        bmp_save(img, "output.bmp");
        bmp_free(img);           // Free memory
    }
    return 0;
}
```

## 🖼️ Visual Demonstrations
Here is the result of the library's processing capabilities:

| Original Input | Processed Output |
| :---: | :---: |
| ![Original](assets/airplane.bmp) | ![Processed](test_output.bmp) |
| *airplane.bmp* | *test_output.bmp* |