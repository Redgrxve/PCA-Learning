#ifndef KMEANS_H
#define KMEANS_H

#include <Eigen/Dense>

class KMeans
{
public:
    KMeans();

    void compute(const Eigen::MatrixXd &data);
};

#endif // KMEANS_H
