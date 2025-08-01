#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <Eigen/Dense>
#include <QStack>

namespace Ui {
class ChartWidget;
}

class Model;
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

    void setModel(Model *model);

    void showTrainData(bool show = true);
    void showTestData(bool show = true);

    void removeFeaturesTabs();
    void removePCATabs();

private:
    void setupFeaturesTabs();
    void setupPCATabs();
    void setupProjectionTabs();

    void connectViewSlots(PCAChartView *view);

    void setSliderValue(int value);
    PCAChartView *currentChartView();

    Ui::ChartWidget *ui;
    Model *m_model{};
    QList<PCAChartView*> m_views;

    QList<int> m_featuresTabsIdeces{};
    QList<int> m_pcaTabsIdeces{};

private slots:
    void onZoomInClicked();
    void onZoomOutClicked();
    void onSliderMoved(int pos);
    void onPerformPCAClicked();
    void onPerformClusterization();
};

#endif // CHARTWIDGET_H
