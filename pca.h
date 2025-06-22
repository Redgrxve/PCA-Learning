#ifndef PCA_H
#define PCA_H

#include <Eigen/Dense>

class PCA
{
public:
    static Eigen::MatrixXd meanSubtraction(const Eigen::MatrixXd &data);
};

#endif // PCA_H
