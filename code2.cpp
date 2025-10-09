#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
using namespace std;
using namespace Eigen;

bool isSymmetric(const Eigen::MatrixXd& A) {
    return A.isApprox(A.transpose());
}

void writeMatrixMarket(const MatrixXd& mat, const std::string& filename) {
    std::ofstream out(filename);
    out << "%%MatrixMarket matrix array real general\n";
    out << mat.rows() << " " << mat.cols() << "\n";
    for (int j = 0; j < mat.cols(); ++j)
        for (int i = 0; i < mat.rows(); ++i)
            out << mat(i, j) << "\n";
    out.close();
}


int main() {
    const int N = 9;
    MatrixXd Ag = MatrixXd::Zero(N, N); // matrice 9x9 di zeri

    // Aggiungiamo le connessioni (usiamo indici da 0 a 8 per Eigen)

    Ag(0,1) = Ag(0,3) = 1;
    Ag(1,0) = Ag(1,2) = 1;
    Ag(2,1) = Ag(2,3) = Ag(2,4) = 1;
    Ag(3,0) = Ag(3,2) = 1;
    Ag(4,2) = Ag(4,5) = Ag(4,7) = Ag(4,8) = 1;
    Ag(5,4) = Ag(5,6) = 1;
    Ag(6,5) = Ag(6,7) = Ag(6,8) = 1;
    Ag(7,4) = Ag(7,6) = Ag(7,8) = 1;
    Ag(8,4) = Ag(8,6) = Ag(8,7) = 1;

    // STAMPA della matrice Ag, DA ELIMINARE__________________________________
    //cout << "Matrice di adiacenza (Ag):\n" << Ag << "\n\n";

    // RICHIESTA 1 --------------------

    // Calcolo della norma di Frobenius
    double frobenius_norm_Ag = Ag.norm();
    cout << "Norma di Frobenius di Ag = " << frobenius_norm_Ag << endl;


    // RICHIESTA 2---------------------

    // Costruisci il vettore vg come somma delle righe di Ag
    VectorXd vg = Ag.rowwise().sum();
    //cout << "\n vg :\n" << vg << endl;
    // Costruisci la matrice diagonale Dg con vg sulla diagonale principale
    MatrixXd Dg = vg.asDiagonal();
    //cout << "\nDg:\n" << Dg << endl;
    MatrixXd Lg = Dg - Ag;
    // cout << "Lg:\n" << Lg << endl;

    // a vector of ones long 9
    VectorXd x = VectorXd::Ones(N);
    VectorXd y = Lg*x;
    double euclidian_norm_y = y.norm();
    //std::cout << "y =  " << (y) << std::endl;
    std::cout << "euclidian_norm_y =  " << (euclidian_norm_y) << std::endl;
    std::cout << "Lg symmetric? " << (isSymmetric(Lg) ? "Yes" : "No") << std::endl;

    // THE NORM OF Y is 0???
    // this is behaving exactly as theory predicts. Short answer: y equals the zero vector
    // and its norm is 0 because the graph Laplacian always annihilates the all-ones vector

    //___________________RICHIESTA 3_____________________________

    SelfAdjointEigenSolver<MatrixXd> es(Lg);
    if (es.info() == Success) {
        cout << "Smallest eigenvalues: ";
        cout << es.eigenvalues().head(3).transpose() << "\n";
    } else {
        cout << "Eigen decomposition failed\n";
    }

    // STAMPA:
    // Lg symmetric? Yes
    //Smallest eigenvalues : -2.04895e-16 (<0 ??? WTF)     0.28031      2
    //___________________reason__________________________________
    //Even though your Laplacian Lg is mathematically symmetric and positive semidefinite,
    //when computed with floating-point arithmetic, round - off errors introduce tiny perturbations:
    //- Floating point double precision has about 15–16 digits of accuracy.
    //- Operations like subtraction (Dg - Ag) and eigenvalue decomposition introduce rounding errors around the order of 1e-15.
    //The true smallest eigenvalue should be 0, but the computed one may appear as −2×10⁻¹⁶, 1×10⁻¹⁵, etc.
    //— all effectively zero within numerical precision.

    double minEigen = es.eigenvalues().minCoeff();
    if (minEigen > -1e-12)
        cout << "Lg is positive semidefinite (within numerical tolerance)" << endl;
    else
        cout << "Lg has a significant negative eigenvalue" << endl;
    //___________________________________________________________

    writeMatrixMarket(Lg, "Lg.mtx");







    return 0;
}




