#include "pcachartview.h"
#include "pcachart.h"
#include "model.h"
#include "kmeans.h"

#include <QScatterSeries>
#include <QLineSeries>

PCAChartView::PCAChartView(QWidget *parent)
    : CustomChartView(parent)
{
    m_chart = new PCAChart;
    setChart(m_chart);
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

    if (m_usePCA) {
        const auto xTitle = QString("PC%1").arg(xIndex + 1);
        const auto yTitle = QString("PC%2").arg(yIndex + 1);
        m_chart->setAxesTitles(xTitle, yTitle);
    } else {
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

    m_chart->setTrainDataSeries(trainDataSeries);
    m_chart->setTestDataSeries(testDataSeries);

    trainDataSeries->setName(tr("Обучающая выборка"));
    testDataSeries->setName(tr("Тестовая выборка"));

    const auto &trainData = m_usePCA ? m_model->Z_train() : m_model->X_train();
    const auto &testData  = m_usePCA ? m_model->Z_test()  : m_model->X_test();

    fillDataSeries(trainDataSeries, trainData);
    fillDataSeries(testDataSeries, testData);

    m_chart->addAndAttachSeries(trainDataSeries);
    m_chart->addAndAttachSeries(testDataSeries);
}

void PCAChartView::showTrainData(bool show)
{
    if (!m_model) return;

    m_chart->showTrainDataSeries(show);
}

void PCAChartView::showTestData(bool show)
{
    if (!m_model) return;

    m_chart->showTestDataSeries(show);
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

void PCAChartView::performKMeans()
{
    const auto &kmeans = m_usePCA ? m_model->kmeans_train_pca() :
                                    m_model->kmeans_train();
    const auto &labels = m_usePCA ? m_model->labels_test_pca() :
                                    m_model->labels_test();
    const auto &data   = m_usePCA ? m_model->Z_test() :
                                    m_model->X_test();
    const int k = kmeans.k();

    QList<QScatterSeries*> clustersSeries;
    clustersSeries.reserve(k);
    for (int i = 0; i < k; ++i) {
        auto series = new QScatterSeries(m_chart);
        series->setColor(generateColor(i));
        clustersSeries.push_back(series);
    }

    if (labels.size() != data.rows()) {
        qWarning() << "Размер labels и Z_test не совпадают!";
        return;
    }

    for (int i = 0; i < data.rows(); ++i) {
        const QPointF point(data(i, m_xIndex), data(i, m_yIndex));
        clustersSeries[labels[i]]->append(point);
    }

    for (int i = 0; i < k; ++i) {
        auto series = clustersSeries[i];
        series->setName(QString("Кластер %1").arg(i+1));
        m_chart->addAndAttachSeries(clustersSeries[i]);
    }
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

QColor PCAChartView::generateColor(int index)
{
    static QList<QColor> baseColors = { Qt::red, Qt::blue, Qt::green, Qt::cyan, Qt::magenta, Qt::yellow };
    if (index < baseColors.size())
        return baseColors[index];

    return QColor::fromHsv((index * 45) % 360, 255, 200);
}

