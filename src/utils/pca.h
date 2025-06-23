#ifndef PCA_H
#define PCA_H

#include <Eigen/Dense>
#include <iostream>
#include <qtypes.h>

namespace PCA
{

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

Eigen::MatrixXd reduceData(const Eigen::MatrixXd &data, const Eigen::MatrixXd &eigVectors, int componentsCount)
{
    Eigen::MatrixXd targetVectors = eigVectors.rightCols(componentsCount);
    return data * targetVectors;
}

Eigen::MatrixXd performPCA(const Eigen::MatrixXd &data, int componentsCount)
{
    const auto centered = meanSubtraction(data);
    std::cout << "\nCentered\n" << centered << std::endl;
    const auto covMatrix = findCovarianceMatrix(centered);
    std::cout << "\nCov\n" << covMatrix << std::endl;
    const auto eigVectors = findEigenVectors(covMatrix, 0);
    std::cout << "\nEigen vectors\n" << eigVectors << std::endl;
    const auto reducedData = reduceData(centered, eigVectors, componentsCount);
    std::cout << "\nProjection\n" << reducedData << std::endl;
    return reducedData;
}
};

#endif // PCA_H
