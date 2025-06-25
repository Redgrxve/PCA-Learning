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
    connect(ui->centeredDataCheckBox, &QCheckBox::clicked,
            this, [this](bool checked){ showCenteredData(checked); });
    connect(ui->reducedDataCheckBox, &QCheckBox::clicked,
            this, [this](bool checked){ showReducedData(checked); });

    connect(ui->performPCAButton, &QPushButton::clicked,
            this, &ChartWidget::onPerformPCAClicked);
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::setModel(PCADataModel *model)
{
    m_model = model;

    setupSeries();

    const auto &initialData = model->initialData();
    const int rows = static_cast<int>(initialData.rows());
    const int cols = static_cast<int>(initialData.cols());
    ui->componentsSpinBox->setMaximum(std::min(rows - 1, cols));

    showInitialData(true);
    showCenteredData(false);
}

void ChartWidget::setupSeries()
{
    if (!m_model) return;

    PCAChart *pcaChart = ui->chartView->pcaChart();
    pcaChart->clearAllDataSeries();

    auto initialDataSeries = new QScatterSeries(pcaChart);
    auto centeredDataSeries = new QScatterSeries(pcaChart);

    const auto &initialData = m_model->initialData();
    const auto &centeredData = m_model->centeredData();

    fillSeriesFromMatrix(initialDataSeries, initialData);
    fillSeriesFromMatrix(centeredDataSeries, centeredData);

    pcaChart->setInitialDataSeries(initialDataSeries);
    pcaChart->setCenteredDataSeries(centeredDataSeries);
}

void ChartWidget::showInitialData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showDataSeries(show);
    ui->initialDataCheckBox->setChecked(show);
}

void ChartWidget::showCenteredData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showCenteredDataSeries(show);
    ui->centeredDataCheckBox->setChecked(show);
}

void ChartWidget::showReducedData(bool show)
{
    if (!m_model) return;

    ui->chartView->pcaChart()->showReducedDataSeries(show);
    ui->reducedDataCheckBox->setChecked(show);
}

void ChartWidget::setSliderValue(int value)
{
    ui->zoomSlider->setValue(value);
    ui->zoomPercentLabel->setText(QString::number(value) + "%");
}

void ChartWidget::fillSeriesFromMatrix(QScatterSeries* series, const Eigen::MatrixXd& matrix)
{
    series->clear();

    if (matrix.rows() == 0) {
        qWarning() << "Matrix is empty";
        return;
    }

    if (matrix.cols() == 1) {
        for (int i = 0; i < matrix.rows(); ++i)
            series->append(matrix(i, 0), 0.0);
    } else if (matrix.cols() >= 2) {
        for (int i = 0; i < matrix.rows(); ++i)
            series->append(matrix(i, 0), matrix(i, 1));
    } else {
        qWarning() << "Unsupported matrix shape: " << matrix.rows() << "x" << matrix.cols();
    }
}

void ChartWidget::onSliderMoved(int pos)
{
    const float zoomValue = pos / 100.f;
    ui->chartView->setZoom(zoomValue);
}

void ChartWidget::onPerformPCAClicked()
{
    if (!m_model) return;

    PCAChart *pcaChart = ui->chartView->pcaChart();
    pcaChart->removeReducedDataSeries();

    auto reducedDataSeries = new QScatterSeries(pcaChart);

    m_model->calculateReducedData(ui->componentsSpinBox->value());
    const auto &reducedData = m_model->reducedData();
    fillSeriesFromMatrix(reducedDataSeries, reducedData);

    pcaChart->setReducedDataSeries(reducedDataSeries);

    showReducedData();
    showInitialData(false);
}
