#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>

struct RegressionModel
{
    double a;
    double b;
    Eigen::Vector2d X;
    Eigen::Vector2d Y;
};

class PCADataModel
{
public:
    PCADataModel() = default;
    PCADataModel(const Eigen::MatrixXd &data);

    void setInitialData(const Eigen::MatrixXd &data);

    inline const Eigen::MatrixXd &initialData()  const { return m_initialData; }
    inline const Eigen::MatrixXd &centeredData() const { return m_centeredData; }
    inline const Eigen::MatrixXd &reducedData()  const { return m_reducedData; }

    inline const Eigen::Matrix4d &initialRegression() const { return m_initialRegression; }
    inline const Eigen::Matrix4d &pcaRegression()     const { return m_pcaRegression; }

    void computeCenteredData();
    void computeReducedData(int componentsCount);
    void computeInitialRegression();

private:
    Eigen::MatrixXd m_initialData{};
    Eigen::MatrixXd m_centeredData{};
    Eigen::MatrixXd m_reducedData{};

    Eigen::Matrix4d m_initialRegression{};
    Eigen::Matrix4d m_pcaRegression{};
};

#endif // PCADATAMODEL_H
