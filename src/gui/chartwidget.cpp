#include "chartwidget.h"
#include "pcachart.h"
#include "pcachartview.h"
#include "pcadatamodel.h"
#include "ui_chartwidget.h"

#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartWidget)
{
    ui->setupUi(this);

    // connect(ui->chartView, &CustomChartView::zoomChanged, this, [this](float value) {
    //     setSliderValue(static_cast<int>(value * 100.f));
    // });

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

    ui->tabWidget->clear();

    const auto &initialData = model->initialData();
    for (qsizetype col = 1; col < initialData.cols(); ++col) {
        auto *view = new PCAChartView(ui->tabWidget);
        view->setModel(model);
        view->setProjectionAxes(0, col);

        const double minX = initialData.col(0).minCoeff();
        const double maxX = initialData.col(0).maxCoeff();
        const double minY = initialData.col(col).minCoeff();
        const double maxY = initialData.col(col).maxCoeff();
        view->setAxesRange(minX, maxX, minY, maxY);

        connect(ui->inButton,  &QPushButton::clicked, view, &CustomChartView::zoomIn);
        connect(ui->outButton, &QPushButton::clicked, view, &CustomChartView::zoomOut);
        connect(view, &CustomChartView::zoomChanged, this, [this](float value) {
            setSliderValue(static_cast<int>(value * 100.f));
        });

        ui->tabWidget->addTab(view, QString("x1 - x%1").arg(col + 1));
    }

    showInitialData(true);
    showInitialRegression(true);
}

void ChartWidget::showInitialData(bool show)
{
    if (!m_model) return;

    currentChartView()->showInitialData(show);
    ui->initialDataCheckBox->setChecked(show);
}

void ChartWidget::showReducedData(bool show)
{
    if (!m_model) return;

    currentChartView()->showReducedData(show);
    ui->reducedDataCheckBox->setChecked(show);
}

void ChartWidget::showInitialRegression(bool show)
{
    if (!m_model) return;

    currentChartView()->showInitialRegression(show);
    ui->initialRegressionCheckBox->setChecked(show);
}

void ChartWidget::showPCARegression(bool show)
{
    if (!m_model) return;

    currentChartView()->showPCARegression(show);
    ui->pcaRegressionCheckBox->setChecked(show);
}

void ChartWidget::setSliderValue(int value)
{
    ui->zoomSlider->setValue(value);
    ui->zoomPercentLabel->setText(QString::number(value) + "%");
}

PCAChartView *ChartWidget::currentChartView()
{
    return static_cast<PCAChartView*>(ui->tabWidget->currentWidget());
}

void ChartWidget::onSliderMoved(int pos)
{
    const float zoomValue = pos / 100.f;
    currentChartView()->setZoom(zoomValue);
}

void ChartWidget::onPerformPCAClicked()
{
    if (!m_model) return;

    currentChartView()->clearPCASeries();

    m_model->computеPCA(ui->componentsSpinBox->value());

    const auto &reducedData = m_model->reducedData();
    for (qsizetype col = 0; col < reducedData.cols(); ++col) {
        auto *view = static_cast<PCAChartView*>(ui->tabWidget->widget(col));
        view->setupPCADataSeries();
    }

    showReducedData();
    showPCARegression();
    showInitialData(false);
    showInitialRegression(false);
}
