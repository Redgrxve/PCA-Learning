#include "chartwidget.h"
#include "pcachart.h"
#include "pcadatamodel.h"
#include "ui_chartwidget.h"

#include <QtCharts/QScatterSeries>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartWidget)
{
    ui->setupUi(this);

    connect(ui->inButton, &QPushButton::clicked, this, [this]() {
        ui->chartView->zoomIn();
    });
    connect(ui->outButton, &QPushButton::clicked, this, [this]() {
        ui->chartView->zoomOut();
    });

    connect(ui->chartView, &CustomChartView::zoomChanged, this, [this](float value) {
        setSliderValue(static_cast<int>(value * 100.f));
    });

    connect(ui->zoomSlider, &QSlider::sliderMoved, this, &ChartWidget::onSliderMoved);

    connect(ui->initialDataCheckBox, &QCheckBox::clicked,
            this, [this](bool checked){ showInitialData(checked); });
    connect(ui->meanDataCheckBox, &QCheckBox::clicked,
            this, [this](bool checked){ showMeanData(checked); });
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::setModel(PCADataModel *model)
{
    m_model = model;

    PCAChart *pcaChart = ui->chartView->pcaChart();
    if (!pcaChart) {
        qDebug() << "void ChartWidget::setModel(PCADataModel *model): pcaChart is nullptr";
        return;
    }

    QScatterSeries *dataSeries = new QScatterSeries(pcaChart);
    QScatterSeries *meanDataSeries = new QScatterSeries(pcaChart);

    dataSeries->setMarkerSize(10.0);
    meanDataSeries->setMarkerSize(10.0);

    const auto &data = model->data();
    for (int i = 0; i < data.rows(); ++i) {
        dataSeries->append(data(i, 0), data(i, 1));
    }

    const auto &meanData = m_model->meanData();
    for (int i = 0; i < meanData.rows(); ++i) {
        meanDataSeries->append(meanData(i, 0), meanData(i, 1));
    }

    pcaChart->setDataSeries(dataSeries);

    pcaChart->setMeanDataSeries(meanDataSeries);
    pcaChart->showMeanDataSeries(false);
}

void ChartWidget::showInitialData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showDataSeries(show);
}

void ChartWidget::showMeanData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showMeanDataSeries(show);
}

void ChartWidget::setSliderValue(int value)
{
    ui->zoomSlider->setValue(value);
    ui->zoomPercentLabel->setText(QString::number(value) + "%");
}

void ChartWidget::onSliderMoved(int pos)
{
    const float zoomValue = pos / 100.f;
    ui->chartView->setZoom(zoomValue);
}
