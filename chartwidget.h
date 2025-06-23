#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>

class PCADataModel;

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

    void showInitialData(bool show = true);
    void showMeanData(bool show = true);
    void showReducedData(bool show = true);


private:
    Ui::ChartWidget *ui;
    PCADataModel *m_model;

    void setSliderValue(int value);

private slots:
    void onSliderMoved(int pos);
};

#endif // CHARTWIDGET_H
