#ifndef LATOOLS_H
#define LATOOLS_H

#include <iostream>
#include <qtypes.h>

#include "jacobieigensolver.h"
#include "types.h"

namespace LATools
{

std::pair<double, Eigen::VectorXd> powerIteration(const Eigen::MatrixXd &a)
{
    double tolerance = 1e-6;
    int maxIter = 1000;

    Eigen::VectorXd v = Eigen::VectorXd::Ones(a.cols());
    v.normalize();

    double lambda = 0.0;
    double lambdaPrev = 0.0;

    for (int i = 0; i < maxIter; ++i) {
        Eigen::VectorXd av = a * v;
        lambda = v.dot(av);
        av.normalize();

        if (abs(lambda - lambdaPrev) < tolerance)
            break;

        v = av;
        lambdaPrev = lambda;
    }

    std::cout << "\nMy Eigen vec\n" << v << std::endl;
    std::cout << "\nMy Eigen val\n" << lambda <<std::endl;

    return {lambda, v};
}


Eigen::MatrixXd meanSubtraction(const Eigen::MatrixXd &data) {
    std::vector<double> avg(data.cols());
    Eigen::MatrixXd result = data;

    for (qsizetype i = 0; i < data.cols(); ++i) {
        for (qsizetype j = 0; j < data.rows(); ++j) {
            avg[i] += data(j, i);
        }
        avg[i] /= data.rows();
    }

    for (qsizetype i = 0; i < data.cols(); ++i) {
        for (qsizetype j = 0; j < data.rows(); ++j) {
            result(j, i) -= avg[i];
        }
    }

    return result;
}

Eigen::MatrixXd findCovarianceMatrix(const Eigen::MatrixXd &centeredData) {
    return (centeredData.transpose() * centeredData) / (centeredData.rows() - 1);
}


Eigen::VectorXd linearRegression(const Eigen::MatrixXd &X_ext, const Eigen::VectorXd &y)
{
    return (X_ext.transpose() * X_ext).ldlt().solve(X_ext.transpose() * y);
}

double MSE(const Eigen::VectorXd &y, const Eigen::VectorXd &y_pred)
{
    return (y - y_pred).squaredNorm() / y.size();
}

PCA fitPCA(const Eigen::MatrixXd& X_train, int numComponents)
{
    PCA pca;

    pca.mean = X_train.colwise().mean();

    const Eigen::MatrixXd X_centered = X_train.rowwise() - pca.mean.transpose();
    const Eigen::MatrixXd covMatrix = LATools::findCovarianceMatrix(X_centered);

    JacobiEigenSolver solver;
    solver.compute(covMatrix);

    const Eigen::MatrixXd eigenvectors = solver.eigenvectors();

    pca.components = eigenvectors.leftCols(numComponents);

#ifdef QT_DEBUG
    std::cout << "\nCentered\n"     << X_centered            << std::endl;
    std::cout << "\nCov\n"          << covMatrix             << std::endl;
    std::cout << "\nEigenvectors\n" << solver.eigenvectors() << std::endl;
    std::cout << "\nEigenvalues\n"  << solver.eigenvalues()  << std::endl;
#endif

    return pca;
}

Eigen::MatrixXd transformPCA(const PCA& pca, const Eigen::MatrixXd& X)
{
    const Eigen::MatrixXd X_centered = X.rowwise() - pca.mean.transpose();
    const Eigen::MatrixXd result = X_centered * pca.components;

#ifdef QT_DEBUG
    std::cout << "\nProjection\n" << result << std::endl;
#endif

    return result;
}

Eigen::MatrixXd addIntercept(const Eigen::MatrixXd& X)
{
    Eigen::MatrixXd X_ext(X.rows(), X.cols() + 1);
    X_ext << Eigen::VectorXd::Ones(X.rows()), X;
    return X_ext;
}

RegressionResult trainLinearRegression(const Eigen::MatrixXd &X_train,
                                       const Eigen::VectorXd &y_train,
                                       const Eigen::MatrixXd &X_test,
                                       const Eigen::VectorXd &y_test)
{
    const auto X_train_ext = LATools::addIntercept(X_train);
    const auto X_test_ext  = LATools::addIntercept(X_test);

    RegressionResult result;
    result.theta = LATools::linearRegression(X_train_ext, y_train);

    result.y_pred_train = X_train_ext * result.theta;
    result.y_pred_test  = X_test_ext  * result.theta;

    result.mse_train = LATools::MSE(y_train, result.y_pred_train);
    result.mse_test  = LATools::MSE(y_test, result.y_pred_test);

    return result;
}

};

#endif // LATOOLS_H
