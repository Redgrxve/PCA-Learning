#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>
#include <QPoint>

struct RegressionModel
{
    Eigen::MatrixXd x;
    Eigen::VectorXd y;
};


class PCADataModel
{
public:
    PCADataModel() = default;
    PCADataModel(const Eigen::MatrixXd &data);

    void setInitialData(const Eigen::MatrixXd &data);

    inline const Eigen::MatrixXd &initialData()  const { return m_initialData; }
    inline const Eigen::MatrixXd &reducedData()  const { return m_reducedData; }

    inline const RegressionModel &initialRegression() const { return m_initialRegression; }
    inline const RegressionModel &pcaRegression()     const { return m_pcaRegression; }

    void computеPCA(int componentsCount);

    void computeInitialRegression();
    void computePCARegression();

private:
    Eigen::MatrixXd m_initialData{};
    Eigen::MatrixXd m_reducedData{};

    Eigen::VectorXd m_inititalPredition;
    Eigen::VectorXd m_pcaPrediction;

    RegressionModel m_initialRegression{};
    RegressionModel m_pcaRegression{};
};

#endif // PCADATAMODEL_H
