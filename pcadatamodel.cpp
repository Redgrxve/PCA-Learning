#include "pcadatamodel.h"
#include "pca.h"
#include <iostream>

PCADataModel::PCADataModel(const Eigen::MatrixXd &data)
    : m_data(data)
{
    m_centeredData = PCA::meanSubtraction(data);
    m_covMatrix = PCA::covarianceMatrix(m_centeredData);
    m_reducedData = PCA::eigenValues(m_centeredData, m_covMatrix);
}

void PCADataModel::calculateCovMatrix()
{
    std::cout << m_data << std::endl;
    std::cout << m_centeredData << std::endl;
    m_covMatrix = PCA::covarianceMatrix(m_centeredData);
    std::cout << m_covMatrix;
}

void PCADataModel::calculateReducedData()
{
    m_reducedData = PCA::eigenValues(m_centeredData, m_covMatrix);
    std::cout << m_reducedData;
}
