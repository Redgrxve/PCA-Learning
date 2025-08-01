#include "kmeans.h"

#include <random>
#include <limits>
#include <numeric>
#include <algorithm>

KMeans::KMeans(int k, int maxIter)
    : m_k(k), m_maxIter(maxIter) {}

void KMeans::compute(const Eigen::MatrixXd &data)
{
    const int n_samples = data.rows();
    const int n_features = data.cols();

    m_centroids = initCentroids(data);

    for (int iter = 0; iter < m_maxIter; ++iter) {
        m_assignments = assignClusters(data);

        Eigen::MatrixXd newCentroids = Eigen::MatrixXd::Zero(m_k, n_features);
        std::vector<int> counts(m_k, 0);

        for (int i = 0; i < n_samples; ++i) {
            const int cluster = m_assignments[i];
            newCentroids.row(cluster) += data.row(i);
            counts[cluster]++;
        }

        for (int i = 0; i < m_k; ++i) {
            if (counts[i] > 0)
                newCentroids.row(i) /= counts[i];
            else
                newCentroids.row(i) = data.row(rand() % n_samples);
        }

        if ((m_centroids - newCentroids).norm() < 1e-4)
            break;

        m_centroids = std::move(newCentroids);
    }
}

Eigen::MatrixXd KMeans::initCentroids(const Eigen::MatrixXd &data)
{
    std::vector<int> indices(data.rows());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

    Eigen::MatrixXd centroids(m_k, data.cols());
    for (int i = 0; i < m_k; ++i)
        centroids.row(i) = data.row(indices[i]);

    return centroids;
}

std::vector<int> KMeans::assignClusters(const Eigen::MatrixXd &data)
{
    const int n_samples = data.rows();

    std::vector<int> labels(n_samples);

    for (int i = 0; i < n_samples; ++i) {
        double minDist = std::numeric_limits<double>::max();
        int bestCluster = 0;

        for (int j = 0; j < m_k; ++j) {
            double dist = (data.row(i) - m_centroids.row(j)).squaredNorm();
            if (dist < minDist) {
                minDist = dist;
                bestCluster = j;
            }
        }

        labels[i] = bestCluster;
    }

    return labels;
}
