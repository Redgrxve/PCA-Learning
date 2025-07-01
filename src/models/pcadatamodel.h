#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>
#include <QPoint>
#include <QStringList>

struct RegressionModel
{
    Eigen::MatrixXd x;
    Eigen::VectorXd y;

    double mse;
};


class PCADataModel
{
public:
    PCADataModel() = default;
    PCADataModel(const Eigen::MatrixXd &data, const QStringList &featuresNames);

    void setFeaturesNames(const QStringList &names) { m_featuresNames = names; }
    void setInitialData(const Eigen::MatrixXd &data);

    inline const QStringList &featuresNames()  const { return m_featuresNames; }

    inline const Eigen::MatrixXd &initialData()  const { return m_initialData; }
    inline const Eigen::MatrixXd &reducedData()  const { return m_reducedData; }

    inline const RegressionModel &initialRegression() const { return m_initialRegression; }
    inline const RegressionModel &pcaRegression()     const { return m_pcaRegression; }

    void computеPCA(int componentsCount);

    void computeInitialRegression();
    void computePCARegression();

private:
    QStringList m_featuresNames{};

    Eigen::MatrixXd m_initialData{};
    Eigen::MatrixXd m_reducedData{};

    Eigen::VectorXd m_inititalPredition;
    Eigen::VectorXd m_pcaPrediction;

    RegressionModel m_initialRegression{};
    RegressionModel m_pcaRegression{};
};

#endif // PCADATAMODEL_H
