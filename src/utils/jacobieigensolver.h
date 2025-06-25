#ifndef JACOBIEIGENSOLVER_H
#define JACOBIEIGENSOLVER_H

#include <Eigen/Dense>

class JacobiEigenSolver
{
public:
    JacobiEigenSolver(double epsilon = 1e-10, int maxIterations = 100);

    bool compute(const Eigen::MatrixXd &inputMatrix);
    void sortDescending();

    inline const Eigen::VectorXd &eigenvalues()  const { return m_eigenvalues; }
    inline const Eigen::MatrixXd &eigenvectors() const { return m_eigenvectors; }

private:
    double m_epsilon;
    int m_maxIterations;

    Eigen::VectorXd m_eigenvalues{};
    Eigen::MatrixXd m_eigenvectors{};
};
#endif // JACOBIEIGENSOLVER_H
