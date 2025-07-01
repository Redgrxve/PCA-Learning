#include "pcachartview.h"
#include "pcachart.h"
#include "pcadatamodel.h"

#include <QScatterSeries>
#include <QLineSeries>

PCAChartView::PCAChartView(QWidget *parent)
    : CustomChartView(parent)
{
    m_chart = new PCAChart;
    setChart(m_chart);

    //
}

PCAChartView::~PCAChartView()
{
    delete m_chart;
}

void PCAChartView::setModel(PCADataModel *model)
{
    m_model = model;
}

void PCAChartView::setProjectionAxes(int xIndex, int yIndex)
{
    m_xIndex = xIndex;
    m_yIndex = yIndex;

    if (!m_usePCA) {
        const auto titles = m_model->featuresNames();
        if (!titles.empty())
            m_chart->setAxesTitles(titles[xIndex], titles[yIndex]);
    }
}

void PCAChartView::setupSeries()
{
    if (!m_model) return;

    m_chart->clearAllDataSeries();

    auto dataSeries = new QScatterSeries(m_chart);
    const auto &data = m_usePCA ? m_model->reducedData() : m_model->initialData();

    fillDataSeries(dataSeries, data);

    if (m_usePCA)
        m_chart->setReducedDataSeries(dataSeries);
    else
        m_chart->setInitialDataSeries(dataSeries);
}

void PCAChartView::showInitialData(bool show)
{
    if (!m_model) return;

    m_chart->showInitialDataSeries(show);
}

void PCAChartView::showReducedData(bool show)
{
    if (!m_model) return;

    m_chart->showReducedDataSeries(show);
}

void PCAChartView::showInitialRegression(bool show)
{
    if (!m_model) return;

    m_chart->showInitialRegression(show);
}

void PCAChartView::showPCARegression(bool show)
{
    if (!m_model) return;

    m_chart->showPCARegression(show);
}

void PCAChartView::clearPCASeries()
{
    m_chart->clearPCADataSeries();
}

void PCAChartView::setAxesTitles(const QString &x, const QString &y)
{
    m_chart->setAxesTitles(x, y);
}

void PCAChartView::setAxesRange(double minX, double maxX, double minY, double maxY)
{
    minX *= m_scaleFactor;
    maxX *= m_scaleFactor;
    minY *= m_scaleFactor;
    maxY *= m_scaleFactor;
    m_chart->setAxisRange(minX, maxX, minY, maxY);
}

void PCAChartView::adjustAxesRange()
{
    const auto &data  = m_usePCA ? m_model->reducedData() : m_model->initialData();
    const double minX = data.col(m_xIndex).minCoeff();
    const double maxX = data.col(m_xIndex).maxCoeff();
    const double minY = data.col(m_yIndex).minCoeff();
    const double maxY = data.col(m_yIndex).maxCoeff();
    setAxesRange(minX, maxX, minY, maxY);
}

void PCAChartView::setupInitialDataSeries()
{
    if (!m_model) return;

    m_chart->clearAllDataSeries();

    auto initialDataSeries = new QScatterSeries(m_chart);
    const auto &initialData = m_model->initialData();

    fillDataSeries(initialDataSeries, initialData);
    m_chart->setInitialDataSeries(initialDataSeries);
}

void PCAChartView::setupPCADataSeries()
{
    if (!m_model) return;

    m_chart->clearPCADataSeries();

    auto reducedDataSeries = new QScatterSeries(m_chart);
    const auto &reducedData = m_model->reducedData();

    fillDataSeries(reducedDataSeries, reducedData);
    m_chart->setReducedDataSeries(reducedDataSeries);
}

void PCAChartView::fillDataSeries(QScatterSeries *series, const Eigen::MatrixXd &matrix)
{
    series->clear();

    if (matrix.rows() == 0) {
        qWarning() << "Matrix is empty";
        return;
    }

    if (m_xIndex >= matrix.cols() ||
        m_yIndex >= matrix.cols()) {
        qWarning() << "X and Y indexes out of bounds";
        return;
    }

    for (int i = 0; i < matrix.rows(); ++i)
        series->append(matrix(i, m_xIndex), matrix(i, m_yIndex));
}

void PCAChartView::fillRegressionSeries(QLineSeries *series, const RegressionModel &regModel)
{
    series->clear();

    QVector<QPointF> points;
    points.reserve(regModel.x.rows());

    for (int i = 0; i < regModel.x.rows(); ++i) {
        points.append(QPointF(regModel.x(i, m_xIndex), regModel.y(i)));
    }

    std::sort(points.begin(), points.end(),
              [](const QPointF &a, const QPointF &b) { return a.x() < b.x(); });

    series->append(points);
}
