#include <Eigen/Dense>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



using namespace Eigen;

int main() {
    const char* image_path = "image/uma.jpg";  // Path to your image
    const char* out_path = "image/uma_noisy.png";

    int width, height, channels;
    // Load image as greyscale (1 channel), width = number of columns, height = number of rows.
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 1);

    if (!data) {
        std::cerr << "Error: Could not load image " << image_path << std::endl;
        return 1;
    }

    std::cout << "Image loaded: " << width << "x" << height << std::endl;

    // Create Eigen matrix (rows = height, cols = width)
    Matrix<unsigned char, Dynamic, Dynamic, RowMajor> image_matrix(height, width);

    // Copy pixel values into matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;  // 1 channel
            image_matrix(i, j) = data[index];
        }
    }

    // Free the image memory
    stbi_image_free(data);

    // Report the size of the matrix
    std::cout << "Matrix size: " << image_matrix.rows() << " x " << image_matrix.cols() << std::endl;

    //______________________________________________RICHIESTA 2
    // Seed RNG
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(-40, 40);

    // Cast to signed type for adding negative noise safely
    Matrix<int, Dynamic, Dynamic, RowMajor> noisy = image_matrix.cast<int>();

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


    //___________________________________________RICHIESTA 3

    VectorXd v = image_matrix.cast<double>().reshaped<Eigen::RowMajor>();
    VectorXd w = noisy.cast<double>().reshaped<Eigen::RowMajor>();
    // Verify the size
    std::cout << "Size of v: " << v.size() << std::endl;
    std::cout << "Size of w: " << w.size() << std::endl;
    // Compute and report Euclidean norm of v
    double norm_v = v.norm();
    std::cout << "Euclidean norm of v: " << norm_v << std::endl;







    
    return 0;
}




