//Load the image as an Eigen matrix with size m × n. Each entry in the matrix corresponds
//to a pixel on the screen and takes a value somewhere between 0 (black) and 255 (white).
//Report the size of the matrix.

#include <Eigen/Dense>
#include <iostream>

// from https://github.com/nothings/stb/tree/master
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
    return 1;
  }

  const char* input_image_path = argv[1];

  // Load the image using stb_image
  int width, height, channels;
  unsigned char* image_data = stbi_load(input_image_path, &width, &height,
                                        &channels, 1);  // Force load as BW
  if (!image_data) {
    std::cerr << "Error: Could not load image " << input_image_path
              << std::endl;
    return 1;
  }

  std::cout << "Image loaded: " << width << "x" << height << " with "
            << channels << " channels." << std::endl;

  // Prepare Eigen matrices for each RGB channel
  MatrixXd mat(height, width);

  // Fill the matrices with image data
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = (i * width + j);  // 1 channel (BW)
      mat(i, j) = static_cast<double>(image_data[index]) / 255.0;
    }
  }
  // Free memory!!!
  stbi_image_free(image_data);

    std::cout << "Matrix size: " << mat.rows() << " x " << mat.cols() << std::endl;

// Introduce a noise signal into the loaded image by adding random fluctuations of color
// ranging between [−40, 40] to each pixel. Export the resulting image in .png and upload it.

 // Create random noise matrix in range [-40, 40]
    MatrixXd noise = (MatrixXd::Random(height, width) * 40.0) / 255;

    // Add noise and clamp between 0 and 255
    MatrixXd noisy = (mat + noise).cwiseMax(0.0).cwiseMin(1.0);

    // Convert back to unsigned char array
    Matrix<unsigned char, Dynamic, Dynamic, RowMajor> noisy_image(height, width);

    // Use Eigen's unaryExpr to map the grayscale values (0.0 to 1.0) to 0 to 255
  noisy_image = noisy.unaryExpr([](double val) -> unsigned char {
    return static_cast<unsigned char>(val * 255.0);
  });
 
  // Save the noisy_image using stb_image_write
  const std::string output_image_path = "output_noisy.png";
  if (stbi_write_png(output_image_path.c_str(), width, height, 1,
                     noisy_image.data(), width) == 0) {
    std::cerr << "Error: Could not save noisy image" << std::endl;
    
    return 1;
  }

  std::cout << "Noisy image saved to " << output_image_path << std::endl;

  return 0;
}
