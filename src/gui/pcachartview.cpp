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
}

void PCAChartView::setupSeries()
{
    if (!m_model) return;

    m_chart->clearAllDataSeries();

    auto dataSeries       = new QScatterSeries(m_chart);
    auto regressionSeries = new QLineSeries(m_chart);

    const auto &data       = m_usePCA ? m_model->reducedData()   : m_model->initialData();
    const auto &regression = m_usePCA ? m_model->pcaRegression() : m_model->initialRegression();

    fillDataSeries(dataSeries, data);
    fillRegressionSeries(regressionSeries, regression);

    if (m_usePCA) {
        m_chart->setReducedDataSeries(dataSeries);
        m_chart->setPCARegressionSeries(regressionSeries);
    } else {
        m_chart->setInitialDataSeries(dataSeries);
        m_chart->setInitialRegressionSeries(regressionSeries);
    }
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
    auto initialRegressionSeries = new QLineSeries(m_chart);

    const auto &initialData = m_model->initialData();
    const auto &initialRegression = m_model->initialRegression();

    fillDataSeries(initialDataSeries, initialData);
    fillRegressionSeries(initialRegressionSeries, initialRegression);

    m_chart->setInitialDataSeries(initialDataSeries);
    m_chart->setInitialRegressionSeries(initialRegressionSeries);
}

void PCAChartView::setupPCADataSeries()
{
    if (!m_model) return;

    m_chart->clearPCADataSeries();

    auto reducedDataSeries = new QScatterSeries(m_chart);
    auto pcaRegressionSeries = new QLineSeries(m_chart);

    const auto &reducedData = m_model->reducedData();
    const auto &pcaRegression = m_model->pcaRegression();

    fillDataSeries(reducedDataSeries, reducedData);
    fillRegressionSeries(pcaRegressionSeries, pcaRegression);

    m_chart->setReducedDataSeries(reducedDataSeries);
    m_chart->setPCARegressionSeries(pcaRegressionSeries);
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

    series->append(regModel.p1);
    series->append(regModel.p2);
}
