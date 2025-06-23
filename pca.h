#ifndef PCA_H
#define PCA_H

#include <Eigen/Dense>
#include <qtypes.h>

namespace PCA
{

static Eigen::MatrixXd meanSubtraction(const Eigen::MatrixXd &data) {
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

static Eigen::MatrixXd covarianceMatrix(const Eigen::MatrixXd &data) {
    return (data.transpose() * data) / (data.rows() - 1);
}

static Eigen::MatrixXd eigenVectors(const Eigen::MatrixXd &centered, const Eigen::MatrixXd &cov, int components) {
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
    if (eig.info() != Eigen::Success) {
        throw std::runtime_error("Eigen decomposition failed");
    }

    if (components == 0)
        return eig.eigenvectors();

    return eig.eigenvectors().rightCols(components);
}

static Eigen::MatrixXd reduceData(const Eigen::MatrixXd &data, const Eigen::MatrixXd &eigVectors)
{
    int k = 2;
    Eigen::MatrixXd targetVectors = eigVectors.rightCols(k);
    return data * targetVectors;
}

};

#endif // PCA_H
