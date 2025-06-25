#include "jacobieigensolver.h"

JacobiEigenSolver::JacobiEigenSolver(double epsilon, int maxIterations)
    : m_epsilon(epsilon), m_maxIterations(maxIterations) {}

bool JacobiEigenSolver::compute(const Eigen::MatrixXd &inputMatrix)
{
    if (inputMatrix.rows() != inputMatrix.cols())
        return false;

    const int n = inputMatrix.rows();
    Eigen::MatrixXd A = inputMatrix;
    Eigen::MatrixXd V = Eigen::MatrixXd::Identity(n, n);

    for (int iter = 0; iter < m_maxIterations; ++iter) {
        int p = 0;
        int q = 1;
        double maxVal = std::abs(A(p, q));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (std::abs(A(i, j)) > maxVal) {
                    maxVal = std::abs(A(i, j));
                    p = i;
                    q = j;
                }
            }
        }

        if (maxVal < m_epsilon)
            break;

        const double App = A(p, p);
        const double Aqq = A(q, q);
        const double Apq = A(p, q);

        const double phi = 0.5 * std::atan2(2.0 * Apq, Aqq - App);
        const double c = std::cos(phi);
        const double s = std::sin(phi);

        Eigen::MatrixXd Anew = A;

        for (int i = 0; i < n; ++i) {
            if (i != p && i != q) {
                const double Aip = c * A(i, p) - s * A(i, q);
                const double Aiq = s * A(i, p) + c * A(i, q);
                Anew(i, p) = Anew(p, i) = Aip;
                Anew(i, q) = Anew(q, i) = Aiq;
            }
        }

        Anew(p, p) = c * c * App - 2 * s * c * Apq + s * s * Aqq;
        Anew(q, q) = s * s * App + 2 * s * c * Apq + c * c * Aqq;
        Anew(p, q) = Anew(q, p) = 0.0;

        A = Anew;

        for (int i = 0; i < n; ++i) {
            const double vip = c * V(i, p) - s * V(i, q);
            const double viq = s * V(i, p) + c * V(i, q);
            V(i, p) = vip;
            V(i, q) = viq;
        }
    }

    m_eigenvalues = A.diagonal();
    m_eigenvectors = V;

    sortDescending();

    return true;
}

void JacobiEigenSolver::sortDescending()
{
    const int n = m_eigenvalues.size();

    std::vector<std::pair<double, int>> indexed;
    indexed.reserve(n);

    for (int i = 0; i < n; ++i) {
        indexed.emplace_back(m_eigenvalues(i), i);
    }

    std::sort(indexed.begin(), indexed.end(), [](auto &a, auto &b) {
        return a.first > b.first;
    });

    Eigen::VectorXd sortedValues(n);
    Eigen::MatrixXd sortedVectors(m_eigenvectors.rows(), n);

    for (int i = 0; i < n; ++i) {
        int idx = indexed[i].second;
        sortedValues(i) = m_eigenvalues(idx);
        sortedVectors.col(i) = m_eigenvectors.col(idx);
    }

    m_eigenvalues = std::move(sortedValues);
    m_eigenvectors = std::move(sortedVectors);
}
