#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <Eigen/Dense>

namespace Ui {
class ChartWidget;
}

class PCADataModel;
class QScatterSeries;
class QLineSeries;
class RegressionModel;

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    void setModel(PCADataModel *model);
    void setupSeries();

    void showInitialData(bool show = true);
    void showCenteredData(bool show = true);
    void showReducedData(bool show = true);
    void showInitialRegression(bool show = true);
    void showPCARegression(bool show = true);

private:
    Ui::ChartWidget *ui;
    PCADataModel *m_model{};

    void setSliderValue(int value);
    void fillScatterSeries(QScatterSeries *series, const Eigen::MatrixXd &matrix);
    void fillRegressionSeries(QLineSeries *series, const RegressionModel &regModel);

private slots:
    void onSliderMoved(int pos);
    void onPerformPCAClicked();
};

#endif // CHARTWIDGET_H
