#include "pcachartview.h"
#include "pcachart.h"
#include "model.h"

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

void PCAChartView::setModel(Model *model)
{
    m_model = model;
}

void PCAChartView::setProjectionAxes(int xIndex, int yIndex)
{
    m_xIndex = xIndex;
    m_yIndex = yIndex;

    if (!m_usePCA) {
        const auto titles = m_model->featureNames();
        if (!titles.empty())
            m_chart->setAxesTitles(titles[xIndex], titles[yIndex]);
    }
}

void PCAChartView::setupSeries()
{
    if (!m_model) return;

    m_chart->clearAllDataSeries();

    auto trainDataSeries = new QScatterSeries(m_chart);
    auto testDataSeries = new QScatterSeries(m_chart);

    trainDataSeries->setName(tr("Обучающая выборка"));
    testDataSeries->setName(tr("Тестовая выборка"));

    const auto &trainData = m_usePCA ? m_model->Z_train() : m_model->X_train();
    const auto &testData =  m_usePCA ? m_model->Z_test()  : m_model->X_test();

    fillDataSeries(trainDataSeries, trainData);
    fillDataSeries(testDataSeries, testData);

    m_chart->addAndAttachSeries(trainDataSeries);
    m_chart->addAndAttachSeries(testDataSeries);



    // auto pred_testDataSeries = new QScatterSeries(m_chart);
    // pred_testDataSeries->setName(tr("Предсказанные данные"));

    // const auto &y_test = m_model->y_test();
    // const auto &y_pred_test = m_model->y_pred_test();
    // for (int i = 0; i < y_test.rows(); ++i)
    //     pred_testDataSeries->append(y_test(i), y_pred_test(i));
    // m_chart->addAndAttachSeries(pred_testDataSeries);
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
    const auto &data  = m_usePCA ? m_model->Z_train() : m_model->X_train();
    const double minX = data.col(m_xIndex).minCoeff();
    const double maxX = data.col(m_xIndex).maxCoeff();
    const double minY = data.col(m_yIndex).minCoeff();
    const double maxY = data.col(m_yIndex).maxCoeff();
    setAxesRange(minX, maxX, minY, maxY);
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

void PCAChartView::fillPredictedSeries(QScatterSeries *series, const Eigen::MatrixXd &X, const Eigen::VectorXd &y_pred)
{
    series->clear();

    for (int i = 0; i < X.rows(); ++i)
        series->append(X(i, m_xIndex), y_pred(i));
}

void PCAChartView::fillRegressionSeries(QLineSeries *series, const Eigen::MatrixXd &X, const Eigen::VectorXd &y)
{
    series->clear();

    QVector<QPointF> points;
    points.reserve(X.rows());

    for (int i = 0; i < X.rows(); ++i) {
        points.append(QPointF(X(i, m_xIndex), y(i)));
    }

   /* std::sort(points.begin(), points.end(),
              [](const QPointF &a, const QPointF &b) { return a.x() < b.x(); })*/;

    series->append(points);
}
