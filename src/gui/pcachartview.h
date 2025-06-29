#ifndef PCACHARTVIEW_H
#define PCACHARTVIEW_H

#include "customchartview.h"
#include <Eigen/Dense>

class PCAChart;
class PCADataModel;
class QScatterSeries;
class QLineSeries;
class RegressionModel;

class PCAChartView : public CustomChartView
{
public:
    explicit PCAChartView(QWidget *parent = nullptr);
    ~PCAChartView();

    inline PCAChart *pcaChart() const { return m_chart; }

    void setModel(PCADataModel *model);
    void setProjectionAxes(int xIndex, int yIndex);

    void setupInitialDataSeries();
    void setupPCADataSeries();

    void showInitialData(bool show = true);
    void showReducedData(bool show = true);
    void showInitialRegression(bool show = true);
    void showPCARegression(bool show = true);

    void clearPCASeries();

    void setAxesRange(double minX, double maxX, double minY, double maxY);

private:
    void fillDataSeries(QScatterSeries *series, const Eigen::MatrixXd &matrix);
    void fillRegressionSeries(QLineSeries *series, const RegressionModel &regModel);

    PCAChart     *m_chart{};
    PCADataModel *m_model{};

    int m_xIndex = 0;
    int m_yIndex = 1;

    float m_scaleFactor = 1.2f;
};

#endif // PCACHARTVIEW_H
