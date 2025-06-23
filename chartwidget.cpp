#include "chartwidget.h"
#include "pcachart.h"
#include "pcadatamodel.h"
#include "ui_chartwidget.h"

#include <QtCharts/QScatterSeries>
#include <iostream>

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


    // m_model->calculateCovMatrix();
    // std::cout << "\nReduced data\n";
    // m_model->calculateReducedData();

    setupSeries();
    //showReducedData(true);
}

void ChartWidget::setupSeries()
{
    PCAChart *pcaChart = ui->chartView->pcaChart();
    if (!pcaChart) {
        qDebug() << "void ChartWidget::setModel(PCADataModel *model): pcaChart is nullptr";
        return;
    }

    auto dataSeries = new QScatterSeries(pcaChart);
    auto meanDataSeries = new QScatterSeries(pcaChart);
    auto reducedDataSeries = new QScatterSeries(pcaChart);

    dataSeries->setMarkerSize(10.0);
    meanDataSeries->setMarkerSize(10.0);
    reducedDataSeries->setMarkerSize(10.0);

    const auto &data = m_model->data();
    for (int i = 0; i < data.rows(); ++i) {
        dataSeries->append(data(i, 0), data(i, 1));
    }

    const auto &meanData = m_model->meanData();
    for (int i = 0; i < meanData.rows(); ++i) {
        meanDataSeries->append(meanData(i, 0), meanData(i, 1));
    }

    const auto &reducedData = m_model->reducedData();
    for (int i = 0; i < reducedData.rows(); ++i) {
        reducedDataSeries->append(reducedData(i, 0), reducedData(i, 1));
    }

    pcaChart->setDataSeries(dataSeries);
    pcaChart->setMeanDataSeries(meanDataSeries);
    pcaChart->setReducedDataSeries(reducedDataSeries);

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

void ChartWidget::showReducedData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showReducedDataSeries(show);
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
