#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>

class PCADataModel
{
public:
    PCADataModel(const Eigen::MatrixXd &data);

    inline void setData(const Eigen::MatrixXd &data) { m_data = data; }

    inline const Eigen::MatrixXd &data()        const { return m_data; }
    inline const Eigen::MatrixXd &meanData()    const { return m_centeredData; }
    inline const Eigen::MatrixXd &reducedData() const { return m_reducedData; }

    void meanSubtract();

private:
    Eigen::MatrixXd m_data{};
    Eigen::MatrixXd m_centeredData{};
    Eigen::MatrixXd m_reducedData{};
};

#endif // PCADATAMODEL_H
