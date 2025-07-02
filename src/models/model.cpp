#include "model.h"
#include "latools.h"
#include <random>

Model::Model(const Eigen::MatrixXd &X, const Eigen::VectorXd &y,
             const QStringList &featureNames, const QString &targetName)
    : m_X_full(X), m_y_full(y), m_featureNames(featureNames), m_targetName(targetName)
{
    //computeInitialRegression();
}

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

    pcaData = LATools::fitPCA(m_X_train, numComponents);
    m_Z_train = LATools::transformPCA(pcaData, m_X_train);
    m_Z_test  = LATools::transformPCA(pcaData, m_X_test);

    trainRegressionPCA();
}

void Model::trainRegression()
{
    const auto reg = LATools::trainLinearRegression(m_X_train, m_y_train, m_X_test, m_y_test);

    m_theta_original = reg.theta;

    m_y_pred_train   = reg.y_pred_train;
    m_y_pred_test    = reg.y_pred_test;

    m_mse_train      = reg.mse_train;
    m_mse_test       = reg.mse_test;

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

// void PCADataModel::setInitialData(const Eigen::MatrixXd &data) {
//     m_initialData = data;
//     computeInitialRegression();
// }

// void PCADataModel::computеPCA(int componentsCount)
// {
//     m_reducedData = LATools::performPCA(m_initialData, false, componentsCount);
//     computePCARegression();
// }

// void PCADataModel::computeInitialRegression()
// {
//     Eigen::MatrixXd X = m_initialData.leftCols(m_initialData.cols() - 1);
//     Eigen::MatrixXd Xb(X.rows(), X.cols() + 1);
//     Xb << Eigen::VectorXd::Ones(X.rows()), X;

//     Eigen::VectorXd y = m_initialData.rightCols(1);
//     Eigen::VectorXd a = LATools::linearRegression(Xb, y);

//     Eigen::VectorXd yPred = Xb * a;

//     m_initialRegression.x = X;
//     m_initialRegression.y = yPred;
//     m_initialRegression.mse = LATools::MSE(y, yPred);
// }

// void PCADataModel::computePCARegression()
// {
//     Eigen::MatrixXd X = m_reducedData.leftCols(m_reducedData.cols() - 1);
//     Eigen::MatrixXd Xb(X.rows(), X.cols() + 1);
//     Xb << Eigen::VectorXd::Ones(X.rows()), X;

//     const auto &y = m_reducedData.rightCols(1);
//     Eigen::VectorXd a = LATools::linearRegression(Xb, y);

//     Eigen::VectorXd yPred = Xb * a;

//     m_pcaRegression.x = X;
//     m_pcaRegression.y = yPred;
//     m_pcaRegression.mse = LATools::MSE(y, yPred);
// }


