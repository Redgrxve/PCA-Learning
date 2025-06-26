#ifndef LATOOLS_H
#define LATOOLS_H

#include <Eigen/Dense>
#include <iostream>
#include <qtypes.h>

#include "jacobieigensolver.h"

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

Eigen::MatrixXd findEigenVectors(const Eigen::MatrixXd &cov, int components) {
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
    if (eig.info() != Eigen::Success) {
        throw std::runtime_error("Eigen decomposition failed");
    }

    if (components == 0)
        return eig.eigenvectors();

    return eig.eigenvectors().rightCols(components);
}

Eigen::MatrixXd reduceData(const Eigen::MatrixXd &data,
                           const Eigen::MatrixXd &eigVectors,
                           int componentsCount)
{
    Eigen::MatrixXd targetVectors = eigVectors.leftCols(componentsCount);
    return data * targetVectors;
}

Eigen::MatrixXd performPCA(const Eigen::MatrixXd &data,
                           bool isCentered = false,
                           int componentsCount = 2)
{
    Eigen::MatrixXd   covMatrix;
    Eigen::MatrixXd   reducedData;
    JacobiEigenSolver jacobiEigen;

    if (!isCentered) {
        const auto centered = meanSubtraction(data);
        covMatrix = findCovarianceMatrix(centered);
        jacobiEigen.compute(covMatrix);
        reducedData = reduceData(centered, jacobiEigen.eigenvectors(), componentsCount);
    } else {
        covMatrix = findCovarianceMatrix(data);
        jacobiEigen.compute(covMatrix);
        reducedData = reduceData(data, jacobiEigen.eigenvectors(), componentsCount);
    }

#ifdef QT_DEBUG
    //std::cout << "\nCentered\n"     << centered                   << std::endl;
    std::cout << "\nCov\n"          << covMatrix                  << std::endl;
    std::cout << "\nEigenvectors\n" << jacobiEigen.eigenvectors() << std::endl;
    std::cout << "\nEigenvalues\n"  << jacobiEigen.eigenvalues()  << std::endl;
    std::cout << "\nProjection\n"   << reducedData                << std::endl;
#endif

    return reducedData;
}

Eigen::VectorXd linearRegression(const Eigen::VectorXd& x, const Eigen::VectorXd& y) {
    Eigen::MatrixXd XwithOnes(x.rows(), x.cols() + 1);
    XwithOnes << x, Eigen::VectorXd::Ones(x.rows());
    return (XwithOnes.transpose() * XwithOnes).ldlt().solve(XwithOnes.transpose() * y);
}

};

#endif // LATOOLS_H
