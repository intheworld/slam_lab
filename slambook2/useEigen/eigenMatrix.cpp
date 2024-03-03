#include <iostream>
#include <ctime>
#include <Eigen/Core>

#include <Eigen/Dense>

#define MATRIX_SIZE 50

using namespace Eigen;

int main(int argc, char **argv) {
    Matrix<float, 2, 3> matrix_23;

    Vector3d v_3d;
    Matrix<float, 3, 1> vd_3d;

    Matrix3d matrix_33 = Matrix3d::Zero();
    Matrix<double, Dynamic, Dynamic> matirx_dynamic;

    MatrixXd matirx_x;

    matrix_23 << 1, 2, 3, 4, 5, 6;

    std::cout << "matrix 2x3 from 1 to 6: \n" << matrix_23 << std::endl;

    std::cout << "print matrix 2x3: " << std::endl;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << matrix_23(i, j) << "\t";
        }
        std::cout << std::endl;
    }

    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;

    Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;

    std::cout << "[1,2,3,4,5,6]*[3,2,1]=" << result.transpose() << std::endl;

    Matrix<float, 2, 1> result2 = matrix_23 * vd_3d;


    // 矩阵运算
    matrix_33 = Matrix3d::Random();
    std::cout << "random matrix: \n" << matrix_33 << std::endl;
    // 转置
    std::cout << "transpose: \n" << matrix_33.sum() << std::endl;
    // 各元素求和
    std::cout << "sum: " << matrix_33.sum() << std::endl;
    // 矩阵的迹
    std::cout << "trace: " << matrix_33.trace() << std::endl;
    // 数乘
    std::cout << "times 10: \n" << 10 * matrix_33 << std::endl;
    // 矩阵的逆
    std::cout << "inverse: \n" << matrix_33.inverse() << std::endl;
    // 行列式
    std::cout << "det: " << matrix_33.determinant() << std::endl;


    // 特征值
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);

    std::cout << "Eigen values = \n" << eigen_solver.eigenvalues() << std::endl;
    std::cout << "Eigen vectors = \n" << eigen_solver.eigenvectors() << std::endl;

    // 解方程
    // matrix_NN ∗ x = v_Nd

    Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN = MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);

    Matrix<double, MATRIX_SIZE, 1> v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);

    // 直接求逆
    clock_t time_stt = clock();
    Matrix<double, MATRIX_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    std::cout << "time of normal inverse is "
        << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << std::endl;
    std::cout << "x = " << x.transpose() << std::endl;

    // QR分解
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);

    std::cout << "time of Qr decomposition is "
        << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << std::endl;
    std::cout << "x = " << x.transpose() << std::endl;

    // 正定矩阵，cholesky分解
    time_stt = clock();
    x = matrix_NN.ldlt().solve(v_Nd);

    std::cout << "time of ldlt decomposition is "
        << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << std::endl;
    std::cout << "x = " << x.transpose() << std::endl;

    return 0;
}
