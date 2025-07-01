#include "pcadatamodel.h"
#include "latools.h"

PCADataModel::PCADataModel(const Eigen::MatrixXd &data, const QStringList &featuresNames)
    : m_initialData(data), m_featuresNames(featuresNames)
{
    computeInitialRegression();
}

void PCADataModel::setInitialData(const Eigen::MatrixXd &data) {
    m_initialData = data;
    computeInitialRegression();
}

void PCADataModel::computеPCA(int componentsCount)
{
    m_reducedData = LATools::performPCA(m_initialData, false, componentsCount);
    computePCARegression();
}

void PCADataModel::computeInitialRegression()
{
    Eigen::MatrixXd X = m_initialData.leftCols(m_initialData.cols() - 1);
    Eigen::MatrixXd Xb(X.rows(), X.cols() + 1);
    Xb << Eigen::VectorXd::Ones(X.rows()), X;

    Eigen::VectorXd y = m_initialData.rightCols(1);
    Eigen::VectorXd a = LATools::linearRegression(Xb, y);

    Eigen::VectorXd yPred = Xb * a;

    m_initialRegression.x = X;
    m_initialRegression.y = yPred;
    m_initialRegression.mse = LATools::MSE(y, yPred);
}

void PCADataModel::computePCARegression()
{
    Eigen::MatrixXd X = m_reducedData.leftCols(m_reducedData.cols() - 1);
    Eigen::MatrixXd Xb(X.rows(), X.cols() + 1);
    Xb << Eigen::VectorXd::Ones(X.rows()), X;

    const auto &y = m_reducedData.rightCols(1);
    Eigen::VectorXd a = LATools::linearRegression(Xb, y);

    Eigen::VectorXd yPred = Xb * a;

    m_pcaRegression.x = X;
    m_pcaRegression.y = yPred;
    m_pcaRegression.mse = LATools::MSE(y, yPred);
}


