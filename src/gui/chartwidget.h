#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <Eigen/Dense>
#include <QStack>

namespace Ui {
class ChartWidget;
}

class PCADataModel;
class QScatterSeries;
class QLineSeries;
class RegressionModel;
class PCAChartView;

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    void setModel(PCADataModel *model);

    void showInitialData(bool show = true);
    void showReducedData(bool show = true);
    void showInitialRegression(bool show = true);
    void showPCARegression(bool show = true);

private:
    void setupRawTabs();
    void setupPCATabs();
    void setupProjectionTabs();

    void connectViewSlots(PCAChartView *view);

    void setSliderValue(int value);
    PCAChartView *currentChartView();

    Ui::ChartWidget *ui;
    PCADataModel *m_model{};
    std::vector<PCAChartView*> m_views;

private slots:
    void onZoomInClicked();
    void onZoomOutClicked();
    void onSliderMoved(int pos);
    void onPerformPCAClicked();
};

#endif // CHARTWIDGET_H
