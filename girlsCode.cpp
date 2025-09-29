//Load the image as an Eigen matrix with size m × n. Each entry in the matrix corresponds
//to a pixel on the screen and takes a value somewhere between 0 (black) and 255 (white).
//Report the size of the matrix.

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>

// from https://github.com/nothings/stb/tree/master
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

bool isSymmetric(const Eigen::SparseMatrix<double>& A, double tol = 1e-9) {
    if (A.rows() != A.cols()) return false;
    for (int k=0; k<A.outerSize(); ++k) {
        for (Eigen::SparseMatrix<double>::InnerIterator it(A,k); it; ++it) {
            int i = it.row();
            int j = it.col();
            double val = it.value();
            double transVal = A.coeff(j,i);
            if (std::abs(val - transVal) != 0.0) return false;
        }
    }
    return true;
}


// int main(int argc, char* argv[]) {
int main() {
  //if (argc < 2) {
  //  std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
  //  return 1;
  //}

    const char* input_image_path = "image/uma.jpg";  // Path to your image
    const char* out_path = "image/uma_noisy.png";
    int width, height, channels;
    // Load image as greyscale (1 channel), width = number of columns, height = number of rows.
    unsigned char* data = stbi_load(input_image_path, &width, &height, &channels, 1);
    if (!data) {
        std::cerr << "Error: Could not load image " << input_image_path << std::endl;
        return 1;
    }



  //const char* input_image_path = argv[1];

  // Load the image using stb_image
  //int width, height, channels;
  double tol=1e-15;
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
    MatrixXd noise = (MatrixXd::Random(height, width)) * (40.0 / 255);

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

// Reshape the original and noisy images as vectors v and w, respectively. Verify that each
// vector has m n components. Report here the Euclidean norm of v.

  VectorXd v = mat.reshaped<Eigen::RowMajor>();
  VectorXd w = noisy.reshaped<Eigen::RowMajor>();

  // std::cout << "Vector v size: " << v.size() << std::endl;
  // std::cout << "Vector w size: " << w.size() << std::endl;
  // //std::cout << "Euclidean norm of w: " << w.norm() << std::endl;

  // //std::cout << "noise: " << noise.col(3) << std::endl;

std::cout << "Expected size: m*n = " << height * width << std::endl;
std::cout << "v.size(): " << (v.size()) << std::endl;
std::cout << "w.size(): " << (w.size()) << std::endl;

 std::cout << "Euclidean norm of v: " << v.norm() << std::endl;

// Write the convolution operation corresponding to the smoothing kernel Hav1 as a matrix
// vector multiplication between a matrix A1 having size mn × mn and the image vector.
// Report the number of non-zero entries in A1 .

// Kernel 3x3 normalizzato
double Hav1[3][3] = {
    {1.0/8, 1.0/8, 0.0/8},
    {1.0/8, 2.0/8, 1.0/8},
    {0.0/8, 1.0/8, 1.0/8}
};

int mn = height * width;

// Create sparse matrix A1 of size (mn x mn)
Eigen::SparseMatrix<double> A1(mn, mn);

// Fill the sparse matrix using triplet list
std::vector<Eigen::Triplet<double>> triplets_A1;

for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
        for (int ki = -1; ki <= 1; ++ki) {
            for (int kj = -1; kj <= 1; ++kj) {
                int ni = i + ki;
                int nj = j + kj;
                if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                    double coeff = Hav1[ki+1][kj+1];
                    if (coeff != 0.0) { // aggiungi solo valori non nulli
                        triplets_A1.emplace_back(i*width + j, ni*width + nj, coeff);
                    }
                }
            }
        }
    }
}

// Set the values in the sparse matrix
A1.setFromTriplets(triplets_A1.begin(), triplets_A1.end());

// Count non-zero entries (ci aspettiamo circa 7*mn= 1 596 000)
int non_zero_count = A1.nonZeros();
std::cout << "Number of non-zero entries in A1: " << non_zero_count << std::endl;


// Apply the previous smoothing filter to the noisy image by performing the matrix vector
// multiplication A1 w. Export and upload the resulting image.

VectorXd smoothedVector = A1 * w;

// Reshape back to matrix
MatrixXd mat_smoothed = (smoothedVector.reshaped<Eigen::RowMajor>(height, width)).cwiseMax(0.0).cwiseMin(1.0);

// Convert back to unsigned char array
  Matrix<unsigned char, Dynamic, Dynamic, RowMajor> smoothed_image(height, width);

  smoothed_image = mat_smoothed.unaryExpr([](double val) -> unsigned char {
  return static_cast<unsigned char>(val * 255.0);
});

// Save the smoothed_image using stb_image_write
const std::string output_smoothed_image_path = "output_smoothed.png";
if (stbi_write_png(output_smoothed_image_path.c_str(), width, height, 1,
                    smoothed_image.data(), width) == 0) {
  std::cerr << "Error: Could not save smoothed image" << std::endl;

  return 1;
}

std::cout << "Smoothed image saved to " << output_smoothed_image_path << std::endl;

// Write the convolution operation corresponding to the sharpening kernel Hsh1 as a matrix
// vector multiplication by a matrix A2 having size mn × mn. Report the number of non-zero
// entries in A2 . Is A2 symmetric?


// Kernel 3x3 normalizzato
double Hsh1[3][3] = {
    {0.0, -2.0, 0.0},
    {-2.0, 9.0, -2.0},
    {0.0, -2.0, 0.0}
};


// Create sparse matrix A1 of size (mn x mn)
Eigen::SparseMatrix<double> A2(mn, mn);

// Fill the sparse matrix using triplet list
std::vector<Eigen::Triplet<double>> triplets_A2;

for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
        for (int ki = -1; ki <= 1; ++ki) {
            for (int kj = -1; kj <= 1; ++kj) {
                int ni = i + ki;
                int nj = j + kj;
                if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                    double coeff = Hsh1[ki+1][kj+1];
                    if (coeff != 0.0) { // aggiungi solo valori non nulli
                        triplets_A2.emplace_back(i*width + j, ni*width + nj, coeff);
                    }
                }
            }
        }
    }
}


// Set the values in the sparse matrix
A2.setFromTriplets(triplets_A2.begin(), triplets_A2.end());

// Count non-zero entries (ci aspettiamo circa 5*mn= 1 400 000)
int non_zero_count_A2 = A2.nonZeros();
std::cout << "Number of non-zero entries in A2: " << non_zero_count_A2 << std::endl;

// Check if A2 is symmetric
std::cout << "A2 symmetric? " << (isSymmetric(A2) ? "Yes" : "No") << std::endl;

// Apply the previous sharpening filter to the original image by performing the matrix vector
// multiplication A2 v. Export and upload the resulting image.

VectorXd sharpenedVector = A2 * v;

// Reshape back to matrix
MatrixXd mat_sharpened = (sharpenedVector.reshaped<Eigen::RowMajor>(height, width)).cwiseMax(0.0).cwiseMin(1.0);

// Convert back to unsigned char array
  Matrix<unsigned char, Dynamic, Dynamic, RowMajor> sharpened_image(height, width);

  sharpened_image = mat_sharpened.unaryExpr([](double val) -> unsigned char {
  return static_cast<unsigned char>(val * 255.0);
});

// Save the sharpened_image using stb_image_write
const std::string output_sharpened_image_path = "output_sharpened.png";
if (stbi_write_png(output_sharpened_image_path.c_str(), width, height, 1,
                    sharpened_image.data(), width) == 0) {
  std::cerr << "Error: Could not save sharpened image" << std::endl;

  return 1;
}

std::cout << "Sharpened image saved to " << output_sharpened_image_path << std::endl;


  return 0;
}

