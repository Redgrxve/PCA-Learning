#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>

class PCADataModel
{
public:
    PCADataModel(const Eigen::MatrixXd &data);

    inline void setData(const Eigen::MatrixXd &data) { m_data = data; }

    inline const Eigen::MatrixXd &data()        const { return m_data; }
    inline const Eigen::MatrixXd &centeredData()    const { return m_centeredData; }
    inline const Eigen::MatrixXd &reducedData() const { return m_reducedData; }

    inline const Eigen::MatrixXd &covMatrix()   const { return m_covMatrix; }

    void meanSubtract();
    void calculateCovMatrix();
    void calculateReducedData();

private:
    Eigen::MatrixXd m_data{};
    Eigen::MatrixXd m_centeredData{};
    Eigen::MatrixXd m_reducedData{};

    Eigen::MatrixXd m_covMatrix;
    Eigen::MatrixXd m_eigVectors;
};

#endif // PCADATAMODEL_H
