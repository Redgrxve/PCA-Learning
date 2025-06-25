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
}

void PCADataModel::computeInitialRegression()
{
    Eigen::VectorXd coeffs = LATools::linearRegression(m_initialData.col(0), m_initialData.col(1));

    double a = coeffs(0);
    double b = coeffs(1);

    double minX = m_initialData.col(0).minCoeff();
    double maxX = m_initialData.col(0).maxCoeff();

    double y1 = a * minX + b;
    double y2 = a * maxX + b;

    m_initialRegression(0, 0) = minX;
    m_initialRegression(0, 1) = y1;
    m_initialRegression(1, 0) = maxX;
    m_initialRegression(1, 1) = y2;
}
