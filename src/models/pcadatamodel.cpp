#include "pcadatamodel.h"
#include "pca.h"

PCADataModel::PCADataModel(const Eigen::MatrixXd &data)
    : m_initialData(data)
{
    m_centeredData = PCA::meanSubtraction(data);
    m_covMatrix = PCA::findCovarianceMatrix(m_centeredData);
    // /m_reducedData = PCA::performPCA(data);
}

void PCADataModel::setInitialData(const Eigen::MatrixXd &data) {
    m_initialData = data;
    m_centeredData = PCA::meanSubtraction(data);
    m_covMatrix = PCA::findCovarianceMatrix(m_centeredData);
}

void PCADataModel::calculateCovMatrix()
{
    m_covMatrix = PCA::findCovarianceMatrix(m_centeredData);
}

void PCADataModel::calculateReducedData(int componentsCount)
{
    m_reducedData = PCA::performPCA(m_initialData, componentsCount);
}
