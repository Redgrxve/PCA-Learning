#include "chartwidget.h"
#include "pcachart.h"
#include "pcachartview.h"
#include "model.h"
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

    connect(ui->inButton,  &QPushButton::clicked, this, &ChartWidget::onZoomInClicked);
    connect(ui->outButton, &QPushButton::clicked, this, &ChartWidget::onZoomOutClicked);

    connect(ui->performPCAButton, &QPushButton::clicked,
            this, &ChartWidget::onPerformPCAClicked);
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::setModel(Model *model)
{
    m_model = model;

    ui->tabWidget->clear();

    setupProjectionTabs();

    const int rows = static_cast<int>(m_model->X_full().rows());
    const int cols = static_cast<int>(m_model->X_full().cols());
    ui->componentsSpinBox->setMaximum(std::min(rows - 1, cols));

    double mseTest = m_model->mse_test();
    double mseTrain = m_model->mse_train();
    ui->initialMseLabel->setText(tr("Before\tMSE train: ") + QString::number(mseTrain) +
                                 tr("\n\tMSE test: ") + QString::number(mseTest));
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

void ChartWidget::setupFeaturesTabs()
{
    if (!m_model) return;

    const qsizetype cols = m_model->X_train().cols();
    for (qsizetype col = 1; col < cols; ++col) {
        auto *view = new PCAChartView(ui->tabWidget);
        view->setModel(m_model);
        view->setUsePCA(false);
        view->setProjectionAxes(0, col);
        view->setupSeries();
        view->adjustAxesRange();

        connectViewSlots(view);

        ui->tabWidget->addTab(view, QString("x1 - x%1").arg(col + 1));
        m_views.push_back(view);
    }
}

void ChartWidget::setupPCATabs()
{
    if (!m_model) return;
    if (m_model->Z_train().cols() < 2) return;

    const qsizetype cols = m_model->Z_train().cols();
    for (qsizetype col = 1; col < cols; ++col) {
        auto *view = new PCAChartView(ui->tabWidget);
        view->setModel(m_model);
        view->setUsePCA(true);
        view->setProjectionAxes(0, col);
        view->setupSeries();
        view->adjustAxesRange();

        connectViewSlots(view);

        ui->tabWidget->addTab(view, QString("PCA: PC1 - PC%1").arg(col + 1));
        m_views.push_back(view);
    }
}

void ChartWidget::setupProjectionTabs()
{
    if (!m_model) return;

    ui->tabWidget->clear();
    m_views.clear();

    setupFeaturesTabs();
    setupPCATabs();
}

void ChartWidget::connectViewSlots(PCAChartView *view)
{
    connect(view, &PCAChartView::zoomChanged, this, [this](float value) {
        setSliderValue(static_cast<int>(value * 100.f));
    });
}

void ChartWidget::onSliderMoved(int pos)
{
    const float zoomValue = pos / 100.f;
    currentChartView()->setZoom(zoomValue);
}

void ChartWidget::onPerformPCAClicked()
{
    if (!m_model) return;

    m_model->applyPCA(ui->componentsSpinBox->value());
    setupProjectionTabs();

    double mseTest = m_model->mse_test_pca();
    double mseTrain = m_model->mse_train_pca();
    ui->pcaMseLabel->setText(tr("After\tMSE train: ") + QString::number(mseTrain) +
                             tr("\n\tMSE test: ") + QString::number(mseTest));
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

void ChartWidget::onZoomInClicked()
{
    if (!currentChartView()) return;

    currentChartView()->zoomIn();
}

void ChartWidget::onZoomOutClicked()
{
    if (!currentChartView()) return;

    currentChartView()->zoomOut();
}
