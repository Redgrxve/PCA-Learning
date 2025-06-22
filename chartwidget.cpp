#include "chartwidget.h"
#include "pca.h"
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

    connect(ui->pushButton, &QPushButton::clicked, this, [this](){
        createMeanChart(m_data);
    });
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::createInitialChart()
{
    QScatterSeries* series = new QScatterSeries();
    series->setMarkerSize(10.0);

    for (int i = 0; i < m_data.rows(); ++i) {
        series->append(m_data(i, 0), m_data(i, 1));
    }

    ui->chartView->customChart()->setMainSeries(series);
}

void ChartWidget::createMeanChart(const Eigen::MatrixXd &data)
{
    ui->chartView->customChart()->removeMainSeries();

    QScatterSeries* series = new QScatterSeries();
    series->setMarkerSize(10.0);

    const auto mean = PCA::meanSubtraction(m_data);
    for (int i = 0; i < m_data.rows(); ++i) {
        series->append(mean(i, 0), mean(i, 1));
    }

    ui->chartView->customChart()->setMainSeries(series);
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
