#include "model.h"
#include "latools.h"
#include "kmeans.h"
#include <random>

Model::Model(const Eigen::MatrixXd &X, const Eigen::VectorXd &y,
             const QStringList &featureNames, const QString &targetName)
    : m_X_full(X), m_y_full(y), m_featureNames(featureNames), m_targetName(targetName)
{}

void Model::setData(const Eigen::MatrixXd &X, const Eigen::VectorXd &y)
{
    m_X_full = X;
    m_y_full = y;

    splitTrainTest();
    trainRegression();
}

void Model::splitTrainTest(double testRatio)
{
    const int totalSamples = m_X_full.rows();
    const int testSize = static_cast<int>(totalSamples * testRatio);
    const int trainSize = totalSamples - testSize;

    std::vector<int> indices(totalSamples);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    m_X_train.resize(trainSize, m_X_full.cols());
    m_X_test.resize(testSize, m_X_full.cols());
    m_y_train.resize(trainSize);
    m_y_test.resize(testSize);

    for (int i = 0; i < trainSize; ++i) {
        m_X_train.row(i) = m_X_full.row(indices[i]);
        m_y_train(i) = m_y_full(indices[i]);
    }
    for (int i = 0; i < testSize; ++i) {
        m_X_test.row(i) = m_X_full.row(indices[trainSize + i]);
        m_y_test(i) = m_y_full(indices[trainSize + i]);
    }
}

void Model::applyPCA(int numComponents)
{
    numComponents = qMin(numComponents, m_X_train.cols());

    pcaData   = LATools::fitPCA(m_X_train, numComponents);
    m_Z_train = LATools::transformPCA(pcaData, m_X_train);
    m_Z_test  = LATools::transformPCA(pcaData, m_X_test);

    trainRegressionPCA();
}

void Model::applyClusterization(int k)
{
    m_clustersData_train_pca.compute(m_Z_train, k);

    const int samples = m_Z_test.rows();
    m_labels_test_pca.clear();
    m_labels_test_pca.reserve(samples);

    for (int i = 0; i < samples; ++i) {
        const Eigen::VectorXd &point = m_Z_test.row(i);
        const int predicted = m_clustersData_train_pca.predict(point);
        m_labels_test_pca.push_back(predicted);
    }
}

void Model::trainRegression()
{
    const auto reg = LATools::trainLinearRegression(m_X_train, m_y_train, m_X_test, m_y_test);

    m_theta_original = reg.theta;

    m_y_pred_train = std::move(reg.y_pred_train);
    m_y_pred_test  = std::move(reg.y_pred_test);

    m_mse_train = reg.mse_train;
    m_mse_test  = reg.mse_test;

    std::cout << "Max diff y vs y_pred (train): " << (m_y_train - m_y_pred_train).cwiseAbs().maxCoeff() << std::endl;
    std::cout << "Max diff y vs y_pred (test): " << (m_y_test  - m_y_pred_test ).cwiseAbs().maxCoeff() << std::endl;
}

void Model::trainRegressionPCA()
{
    const auto reg = LATools::trainLinearRegression(m_Z_train, m_y_train, m_Z_test, m_y_test);

    m_theta_pca = reg.theta;

    m_y_pred_train_pca = reg.y_pred_train;
    m_y_pred_test_pca  = reg.y_pred_test;

    m_mse_train_pca    = reg.mse_train;
    m_mse_test_pca     = reg.mse_test;
}


