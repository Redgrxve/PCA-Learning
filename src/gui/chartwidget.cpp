#include "chartwidget.h"
#include "pcachart.h"
#include "pcadatamodel.h"
#include "ui_chartwidget.h"

#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

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
            this, &ChartWidget::showInitialData);
    connect(ui->reducedDataCheckBox, &QCheckBox::clicked,
            this, &ChartWidget::showReducedData);
    connect(ui->initialRegressionCheckBox, &QCheckBox::clicked,
            this, &ChartWidget::showInitialRegression);
    connect(ui->pcaRegressionCheckBox, &QCheckBox::clicked,
            this, &ChartWidget::showPCARegression);

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

    ui->chartView->setModel(model);
    ui->chartView->setProjectionAxes(0, 1);

    const double minX = model->initialData().col(0).minCoeff();
    const double maxX = model->initialData().col(0).maxCoeff();
    const double minY = model->initialData().col(1).minCoeff();
    const double maxY = model->initialData().col(1).maxCoeff();
    ui->chartView->setAxesRange(minX, maxX, minY, maxY);

    showInitialData(true);
    showInitialRegression(true);
}

void ChartWidget::showInitialData(bool show)
{
    if (!m_model) return;

    ui->chartView->showInitialData(show);
    ui->initialDataCheckBox->setChecked(show);
}

void ChartWidget::showReducedData(bool show)
{
    if (!m_model) return;

    ui->chartView->showReducedData(show);
    ui->reducedDataCheckBox->setChecked(show);
}

void ChartWidget::showInitialRegression(bool show)
{
    if (!m_model) return;

    ui->chartView->showInitialRegression(show);
    ui->initialRegressionCheckBox->setChecked(show);
}

void ChartWidget::showPCARegression(bool show)
{
    if (!m_model) return;

    ui->chartView->showPCARegression(show);
    ui->pcaRegressionCheckBox->setChecked(show);
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

void ChartWidget::onPerformPCAClicked()
{
    if (!m_model) return;

    m_model->computеPCA(ui->componentsSpinBox->value());
    ui->chartView->setupPCADataSeries();

    showReducedData();
    showPCARegression();
    showInitialData(false);
    showInitialRegression(false);
}
