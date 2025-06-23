#include "pcadatamodel.h"
#include "pca.h"

PCADataModel::PCADataModel(const Eigen::MatrixXd &data)
    : m_data(data)
{
    m_centeredData = PCA::meanSubtraction(m_data);
}
