#include "pcadatamodel.h"
#include "latools.h"

PCADataModel::PCADataModel(const Eigen::MatrixXd &data)
    : m_initialData(data)
{
    m_centeredData = LATools::meanSubtraction(data);
    computeInitialRegression();
}

void PCADataModel::setInitialData(const Eigen::MatrixXd &data) {
    m_initialData = data;
    m_centeredData = LATools::meanSubtraction(data);
    computeInitialRegression();
}

void PCADataModel::computeReducedData(int componentsCount)
{
    m_reducedData = LATools::performPCA(m_centeredData, true, componentsCount);
    computePCARegression();
}

void PCADataModel::computeInitialRegression()
{
    Eigen::VectorXd x = m_initialData.col(0);
    Eigen::VectorXd y = m_initialData.col(1);
    Eigen::VectorXd coeffs = LATools::linearRegression(x, y);

    double a = coeffs(0);
    double b = coeffs(1);

    double minX = x.minCoeff();
    double maxX = x.maxCoeff();

    double y1 = a * minX + b;
    double y2 = a * maxX + b;

    m_initialRegression.setCoords(minX, y1, maxX, y2);
}

void PCADataModel::computePCARegression()
{
    Eigen::VectorXd x = m_reducedData.col(0);
    Eigen::VectorXd y;

    if (m_reducedData.cols() == 1)
        y = m_initialData.col(1);
    else
        y = m_reducedData.col(1);

    Eigen::VectorXd coeffs = LATools::linearRegression(x, y);

    double a = coeffs(0);
    double b = coeffs(1);

    double minX = x.minCoeff();
    double maxX = x.maxCoeff();

    double y1 = a * minX + b;
    double y2 = a * maxX + b;

    m_pcaRegression.setCoords(minX, y1, maxX, y2);
}
