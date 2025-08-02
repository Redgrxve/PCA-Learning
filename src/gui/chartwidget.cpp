#include "chartwidget.h"
#include "kmeans.h"
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

    connect(ui->zoomSlider, &QSlider::sliderMoved, this, &ChartWidget::onSliderMoved);

    connect(ui->trainDataCheckBox, &QCheckBox::clicked,
             this, &ChartWidget::showTrainData);
    connect(ui->testDataCheckBox, &QCheckBox::clicked,
             this, &ChartWidget::showTestData);

    connect(ui->inButton,  &QPushButton::clicked, this, &ChartWidget::onZoomInClicked);
    connect(ui->outButton, &QPushButton::clicked, this, &ChartWidget::onZoomOutClicked);

    connect(ui->performPCAButton, &QPushButton::clicked,
            this, &ChartWidget::onPerformPCAClicked);
    connect(ui->clusterizationButton, &QPushButton::clicked,
            this, &ChartWidget::onPerformClusterization);
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::setModel(Model *model)
{
    m_model = model;

    setupProjectionTabs();

    const int rows = static_cast<int>(m_model->X_full().rows());
    const int cols = static_cast<int>(m_model->X_full().cols());
    ui->componentsSpinBox->setMaximum(std::min(rows - 1, cols));

    double mseTest = m_model->mse_test();
    double mseTrain = m_model->mse_train();
    ui->initialMseLabel->setText(tr("До\tMSE train: ") + QString::number(mseTrain) +
                                 tr("\n\tMSE test: ") + QString::number(mseTest));
}

void ChartWidget::showTrainData(bool show)
{
    if (!m_model) return;

    currentChartView()->showTrainData(show);
    ui->trainDataCheckBox->setChecked(show);
}

void ChartWidget::showTestData(bool show)
{
    if (!m_model) return;

    currentChartView()->showTestData(show);
    ui->testDataCheckBox->setChecked(show);
}

void ChartWidget::removeFeaturesTabs()
{
    if (m_featuresTabsIdeces.empty()) return;

    std::sort(m_featuresTabsIdeces.rbegin(), m_featuresTabsIdeces.rend());

    for (const int idx : std::as_const(m_featuresTabsIdeces))
        ui->tabWidget->removeTab(idx);

    m_featuresTabsIdeces.clear();
}

void ChartWidget::removePCATabs()
{
    if (m_pcaTabsIdeces.empty()) return;

    std::sort(m_pcaTabsIdeces.rbegin(), m_pcaTabsIdeces.rend());

    for (const int idx : std::as_const(m_pcaTabsIdeces))
        ui->tabWidget->removeTab(idx);

    m_pcaTabsIdeces.clear();
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

        m_views.append(view);

        const auto tabLabel = QString("x1 - x%1").arg(col + 1);
        const int newTabIndex = ui->tabWidget->addTab(view, tabLabel);
        m_featuresTabsIdeces.append(newTabIndex);
    }
}

void ChartWidget::setupPCATabs()
{
    if (!m_model) return;

    const qsizetype cols = m_model->Z_train().cols();
    for (qsizetype col = 1; col < cols; ++col) {
        auto *view = new PCAChartView(ui->tabWidget);
        view->setModel(m_model);
        view->setUsePCA(true);
        view->setProjectionAxes(0, col);
        view->setupSeries();
        view->adjustAxesRange();

        connectViewSlots(view);

        m_views.append(view);

        const auto tabLabel = QString("PCA: PC1 - PC%1").arg(col + 1);
        const int newTabIndex = ui->tabWidget->addTab(view, tabLabel);
        m_pcaTabsIdeces.append(newTabIndex);
    }
}

void ChartWidget::setupProjectionTabs()
{
    if (!m_model) return;

    ui->tabWidget->clear();
    m_views.clear();

    setupFeaturesTabs();
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
    removePCATabs();
    setupPCATabs();

    const double mseTest = m_model->mse_test_pca();
    const double mseTrain = m_model->mse_train_pca();
    ui->pcaMseLabel->setText(tr("После\tMSE train: ") + QString::number(mseTrain) +
                             tr("\n\tMSE test: ") + QString::number(mseTest));
}

void ChartWidget::onPerformClusterization()
{
    if (!m_model) return;

    if (m_model->Z_train().size() == 0 ||
        m_model->Z_train().cols() > 2) {
        ui->componentsSpinBox->setValue(2);
        onPerformPCAClicked();
    }

    m_model->applyClusterization(ui->clustersSpinBox->value());

    auto *view = new PCAChartView(ui->tabWidget);
    view->setModel(m_model);
    view->setUsePCA(true);
    view->setProjectionAxes(0, 1);
    view->performClusterization();
    view->adjustAxesRange();

    connectViewSlots(view);

    m_views.append(view);

    const auto tabLabel = QString("PCA Кластеры");
    const int newTabIndex = ui->tabWidget->addTab(view, tabLabel);
    m_pcaTabsIdeces.append(newTabIndex);

    ui->tabWidget->setCurrentIndex(newTabIndex);
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
