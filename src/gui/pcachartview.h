#ifndef PCACHARTVIEW_H
#define PCACHARTVIEW_H

#include "customchartview.h"
#include <Eigen/Dense>

class PCAChart;
class Model;
class QScatterSeries;
class QLineSeries;
class RegressionModel;
class KMeans;

class PCAChartView : public CustomChartView
{
public:
    explicit PCAChartView(QWidget *parent = nullptr);
    ~PCAChartView();

    inline PCAChart *pcaChart() const { return m_chart; }

    void setModel(Model *model);
    void setProjectionAxes(int xIndex, int yIndex);

    inline void setUsePCA(bool usePCA) { m_usePCA = usePCA; }

    void setupSeries();

    void showTrainData(bool show = true);
    void showTestData(bool show = true);

    void setAxesTitles(const QString &x, const QString &y);
    void setAxesRange(double minX, double maxX, double minY, double maxY);
    void adjustAxesRange();

    void performClusterization();

private:
    void fillDataSeries(QScatterSeries *series, const Eigen::MatrixXd &matrix);

    QColor generateColor(int index);

    PCAChart *m_chart{};
    Model *m_model{};

    bool m_usePCA = false;

    int m_xIndex = 0;
    int m_yIndex = 1;

    float m_scaleFactor = 1.2f;
};

#endif // PCACHARTVIEW_H
