#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>

class PCADataModel
{
public:
    PCADataModel() = default;
    PCADataModel(const Eigen::MatrixXd &data);

    void setInitialData(const Eigen::MatrixXd &data);

    inline const Eigen::MatrixXd &initialData()        const { return m_initialData; }
    inline const Eigen::MatrixXd &centeredData()    const { return m_centeredData; }
    inline const Eigen::MatrixXd &reducedData() const { return m_reducedData; }

    inline const Eigen::MatrixXd &covMatrix()   const { return m_covMatrix; }

    void meanSubtract();
    void calculateCovMatrix();
    void calculateReducedData(int componentsCount);

private:
    Eigen::MatrixXd m_initialData{};
    Eigen::MatrixXd m_centeredData{};
    Eigen::MatrixXd m_reducedData{};

    Eigen::MatrixXd m_covMatrix;
    Eigen::MatrixXd m_eigVectors;
};

#endif // PCADATAMODEL_H
