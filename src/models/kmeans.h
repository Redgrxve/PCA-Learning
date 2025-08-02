#ifndef KMEANS_H
#define KMEANS_H

#include <Eigen/Dense>

class KMeans
{
public:
    KMeans(int maxIter = 100);

    void compute(const Eigen::MatrixXd &data, int k);

    const std::vector<int> &labels()   const { return m_assignments; }
    const Eigen::MatrixXd &centroids() const { return m_centroids; }
    int k() const { return m_k; }

private:
    int m_k;
    int m_maxIter;
    Eigen::MatrixXd m_centroids;
    std::vector<int> m_assignments;

    Eigen::MatrixXd initCentroids(const Eigen::MatrixXd &data);
    std::vector<int> assignClusters(const Eigen::MatrixXd &data);
};

#endif // KMEANS_H
