#ifndef MODEL_H
#define MODEL_H

#include <QPoint>
#include <QStringList>

#include "types.h"

class Model
{
public:
    Model() = default;
    Model(const Eigen::MatrixXd &X, const Eigen::VectorXd &y,
          const QStringList &featureNames, const QString &targetName);


    void setData(const Eigen::MatrixXd &X, const Eigen::VectorXd &y);
    void splitTrainTest(double testRatio = 0.2);
    void applyPCA(int numComponents);
    void trainRegression();
    void trainRegressionPCA();
    void computeMetrics();

    void setFeaturesNames(const QStringList &names) { m_featureNames = names; }
    void setTargetName(const QString &name) { m_targetName = name; }
    void computеPCA(int componentsCount);

    const QStringList &featureNames() const { return m_featureNames; }

    const Eigen::MatrixXd &X_full()  const { return m_X_full; }
    const Eigen::MatrixXd &X_train() const { return m_X_train; }
    const Eigen::MatrixXd &X_test()  const { return m_X_test; }

    const Eigen::VectorXd &y_train() const { return m_y_train; }
    const Eigen::VectorXd &y_test()  const { return m_y_test; }

    const Eigen::MatrixXd &Z_train() const { return m_Z_train; }
    const Eigen::MatrixXd &Z_test()  const { return m_Z_test; }

    const Eigen::VectorXd &y_pred_train() const { return m_y_pred_train; }
    const Eigen::VectorXd &y_pred_test()  const { return m_y_pred_test; }

    const Eigen::VectorXd &y_pred_train_pca() const { return m_y_pred_train_pca; }
    const Eigen::VectorXd &y_pred_test_pca()  const { return m_y_pred_test_pca; }

    double mse_train() const { return m_mse_train; }
    double mse_test()  const { return m_mse_test; }

    double mse_train_pca() const { return m_mse_train_pca; }
    double mse_test_pca()  const { return m_mse_test_pca; }

private:
    QStringList m_featureNames{};
    QString     m_targetName;     // название целевой переменной (y)

    //Original data
    Eigen::MatrixXd m_X_full;
    Eigen::VectorXd m_y_full;

    //Splitted data
    Eigen::MatrixXd m_X_train;
    Eigen::VectorXd m_y_train;
    Eigen::MatrixXd m_X_test;
    Eigen::VectorXd m_y_test;

    //PCA
    PCA pcaData;

    Eigen::MatrixXd m_Z_train; // PCA(X_train) -> Z_train (n × k)
    Eigen::MatrixXd m_Z_test;  // PCA(X_test)  -> Z_test (n × k)

    //Regression coeffs
    Eigen::VectorXd m_theta_original;
    Eigen::VectorXd m_theta_pca;

    //Predictions
    Eigen::VectorXd m_y_pred_train;
    Eigen::VectorXd m_y_pred_test;
    Eigen::VectorXd m_y_pred_train_pca;
    Eigen::VectorXd m_y_pred_test_pca;

    double m_mse_train = -1.0;
    double m_mse_test = -1.0;

    double m_mse_train_pca = -1.0;
    double m_mse_test_pca = -1.0;
};

#endif // MODEL_H
