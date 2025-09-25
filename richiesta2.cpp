
// add_noise.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

using namespace Eigen;

int main() {
    // Hardcoded input and output paths
    const char* image_path = "image/uma.jpg";
    const char* out_path = "image/uma_noisy.png";

    int width, height, channels;
    // request 1 channel (grayscale)
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 1);
    if (!data) {
        std::cerr << "Error: couldn't load " << image_path << "\n";
        return 1;
    }

    // store as Eigen matrix (rows = height, cols = width)
    Matrix<unsigned char, Dynamic, Dynamic, RowMajor> img(height, width);
    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            img(r, c) = data[r * width + c];

    stbi_image_free(data);

    // Seed RNG
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(-40, 40);

    // Cast to signed type for adding negative noise safely
    Matrix<int, Dynamic, Dynamic, RowMajor> noisy = img.cast<int>();

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            noisy(r, c) += dist(rng);
            if (noisy(r, c) < 0) noisy(r, c) = 0;
            else if (noisy(r, c) > 255) noisy(r, c) = 255;
        }
    }

    // Convert back to unsigned char buffer (row-major contiguous)
    std::vector<unsigned char> outbuf(width * height);
    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            outbuf[r * width + c] = static_cast<unsigned char>(noisy(r, c));

    // write PNG (1 channel). stride = width * channels = width
    if (!stbi_write_png(out_path, width, height, 1, outbuf.data(), width)) {
        std::cerr << "Error: couldn't write " << out_path << "\n";
        return 1;
    }

    std::cout << "Wrote noisy image to " << out_path << "\n";
    return 0;
}