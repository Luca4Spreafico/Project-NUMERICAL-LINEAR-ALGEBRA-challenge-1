#include <Eigen/Dense>
#include <iostream>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

int main() {
    const char* image_path = "image/uma.jpg";  // Path to your image

    int width, height, channels;
    // Load image as greyscale (1 channel), width = number of columns, height = number of rows.
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 1);

    if (!data) {
        std::cerr << "Error: Could not load image " << image_path << std::endl;
        return 1;
    }

    std::cout << "Image loaded: " << width << "x" << height << std::endl;

    // Create Eigen matrix (rows = height, cols = width)
    Matrix<unsigned char, Dynamic, Dynamic, RowMajor> img(height, width);

    // Copy pixel values into matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;  // 1 channel
            img(i, j) = data[index];
        }
    }

    // Free the image memory
    stbi_image_free(data);

    // Report the size of the matrix
    std::cout << "Matrix size: " << img.rows() << " x " << img.cols() << std::endl;

    return 0;
}