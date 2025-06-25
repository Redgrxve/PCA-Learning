#include "pcadatamodel.h"
#include "latools.h"

PCADataModel::PCADataModel(const Eigen::MatrixXd &data)
    : m_initialData(data)
{
    m_centeredData = LATools::meanSubtraction(data);
}

void PCADataModel::setInitialData(const Eigen::MatrixXd &data) {
    m_initialData = data;
    m_centeredData = LATools::meanSubtraction(data);
    m_covMatrix = LATools::findCovarianceMatrix(m_centeredData);
}

void PCADataModel::calculateCovMatrix()
{
    m_covMatrix = LATools::findCovarianceMatrix(m_centeredData);
}

void PCADataModel::calculateReducedData(int componentsCount)
{
    m_reducedData = LATools::performPCA(m_centeredData, true, componentsCount);
}
