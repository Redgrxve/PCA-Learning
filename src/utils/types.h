#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>

struct PCA
{
    Eigen::VectorXd mean;
    Eigen::MatrixXd components;
};

struct RegressionResult {
    Eigen::VectorXd theta;
    Eigen::VectorXd y_pred_train;
    Eigen::VectorXd y_pred_test;
    double mse_train = -1.0;
    double mse_test  = -1.0;
};

#endif // TYPES_H
