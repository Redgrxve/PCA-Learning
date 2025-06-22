#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <Eigen/Dense>

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    inline void setData(const Eigen::MatrixXd &data) { m_data = data; }

    void createInitialChart();
    void createMeanChart(const Eigen::MatrixXd &data);

private:
    Ui::ChartWidget *ui;
    Eigen::MatrixXd m_data;

    void setSliderValue(int value);

private slots:
    void onSliderMoved(int pos);
};

#endif // CHARTWIDGET_H
