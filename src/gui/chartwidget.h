#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <Eigen/Dense>

class PCADataModel;
class QScatterSeries;

namespace Ui {
class ChartWidget;
}

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

private:
    Ui::ChartWidget *ui;
    PCADataModel *m_model{};

    void setSliderValue(int value);
    void fillSeriesFromMatrix(QScatterSeries* series, const Eigen::MatrixXd& matrix);

private slots:
    void onSliderMoved(int pos);
    void onPerformPCAClicked();
};

#endif // CHARTWIDGET_H
