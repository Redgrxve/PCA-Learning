#ifndef PCADATAMODEL_H
#define PCADATAMODEL_H

#include <Eigen/Dense>
#include <QPoint>

struct RegressionModel
{
    QPointF p1;
    QPointF p2;

    void setCoords(double x1, double y1, double x2, double y2) {
        p1.setX(x1);
        p1.setY(y1);
        p2.setX(x2);
        p2.setY(y2);
    }
};

class PCADataModel
{
public:
    PCADataModel() = default;
    PCADataModel(const Eigen::MatrixXd &data);

    void setInitialData(const Eigen::MatrixXd &data);

    inline const Eigen::MatrixXd &initialData()  const { return m_initialData; }
    inline const Eigen::MatrixXd &reducedData()  const { return m_reducedData; }

    inline const RegressionModel &initialRegression() const { return m_initialRegression; }
    inline const RegressionModel &pcaRegression()     const { return m_pcaRegression; }

    void computеPCA(int componentsCount);

    void computeInitialRegression();
    void computePCARegression();

private:
    Eigen::MatrixXd m_initialData{};
    Eigen::MatrixXd m_reducedData{};

    RegressionModel m_initialRegression{};
    RegressionModel m_pcaRegression{};
};

#endif // PCADATAMODEL_H
