#include "pcachart.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>


PCAChart::PCAChart(QGraphicsItem *parent)
    : QChart(parent)
{
    xAxisLine = new QLineSeries();
    yAxisLine = new QLineSeries();

    setAxesLinesColor(Qt::black);

    axisX = new QValueAxis();
    axisY = new QValueAxis();

    addAxis(axisX, Qt::AlignBottom);
    addAxis(axisY, Qt::AlignLeft);

    addSeries(xAxisLine);
    addSeries(yAxisLine);

    xAxisLine->attachAxis(axisX);
    xAxisLine->attachAxis(axisY);
    yAxisLine->attachAxis(axisX);
    yAxisLine->attachAxis(axisY);

    showAxesLines(false);
}

void PCAChart::setInitialDataSeries(QScatterSeries *series)
{
    m_initialDataSeries = series;
    m_initialDataSeries->setName(tr("Исходные данные"));
    addAndAttachSeries(series);
}

void PCAChart::setReducedDataSeries(QScatterSeries *series)
{
    m_reducedDataSeries = series;
    m_reducedDataSeries->setName(tr("Проекция после PCA"));
    addAndAttachSeries(series);
}

void PCAChart::clearInitialDataSeries()
{
    if (!m_initialDataSeries) return;

    deleteSeries(m_initialDataSeries);
}

void PCAChart::clearReducedDataSeries()
{
    if (!m_reducedDataSeries) return;

    deleteSeries(m_reducedDataSeries);
}

void PCAChart::clearPCADataSeries()
{
    clearReducedDataSeries();

    if (m_pcaRegressionSeries)
        deleteSeries(m_pcaRegressionSeries);
}

void PCAChart::adjustAxesRange()
{

}

void PCAChart::addAndAttachSeries(QAbstractSeries *series)
{
    addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void PCAChart::addAndAttachSeries(QScatterSeries *series)
{
    addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    series->setMarkerSize(m_scatterSeriesMarkerSize);
}

void PCAChart::addAndAttachSeries(QLineSeries *series)
{
    addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QPen pen(m_lineSeriesColor);
    pen.setWidth(m_lineSeriesWidth);
    series->setPen(pen);
}

void PCAChart::setInitialRegressionSeries(QLineSeries *series)
{
    m_initialRegressionSeries = series;
    m_initialRegressionSeries->setName("Регрессия до PCA");
    addAndAttachSeries(m_initialRegressionSeries);
}

void PCAChart::setPCARegressionSeries(QLineSeries *series)
{
    m_pcaRegressionSeries = series;
    m_pcaRegressionSeries->setName("Регрессия после PCA");
    addAndAttachSeries(m_pcaRegressionSeries);
}

void PCAChart::setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);

    axisX->setTickCount(m_axesXTickCount);
    axisY->setTickCount(m_axesYTickCount);

    xAxisLine->clear();
    yAxisLine->clear();

    xAxisLine->append(minX, 0);
    xAxisLine->append(maxX, 0);

    yAxisLine->append(0, minY);
    yAxisLine->append(0, maxY);

    setAxesLinesColor(Qt::black);
}

void PCAChart::setAxesLinesColor(const QColor &color)
{
    QPen axisPen(m_axesLinesColor);
    axisPen.setWidth(m_axesLinesWith);

    xAxisLine->setPen(axisPen);
    yAxisLine->setPen(axisPen);

    xAxisLine->setPointsVisible(false);
    yAxisLine->setPointsVisible(false);
}

void PCAChart::showInitialDataSeries(bool show)
{
    if (!m_initialDataSeries) return;

    if (show)
        m_initialDataSeries->show();
    else
        m_initialDataSeries->hide();
}

void PCAChart::showReducedDataSeries(bool show)
{
    if (!m_reducedDataSeries) return;

    if (show)
        m_reducedDataSeries->show();
    else
        m_reducedDataSeries->hide();
}

void PCAChart::showInitialRegression(bool show)
{
    if (!m_initialRegressionSeries) return;

    if (show)
        m_initialRegressionSeries->show();
    else
        m_initialRegressionSeries->hide();
}

void PCAChart::showPCARegression(bool show)
{
    if (!m_pcaRegressionSeries) return;

    if (show)
        m_pcaRegressionSeries->show();
    else
        m_pcaRegressionSeries->hide();
}

void PCAChart::showAxesLines(bool show)
{
    if (!xAxisLine ||
        !yAxisLine) return;

    if (show) {
        xAxisLine->show();
        yAxisLine->show();
    } else {
        xAxisLine->hide();
        yAxisLine->hide();
    }
}

void PCAChart::clearAllDataSeries()
{
    if (m_initialDataSeries)
        deleteSeries(m_initialDataSeries);

    if (m_reducedDataSeries)
        deleteSeries(m_reducedDataSeries);

    if (m_initialRegressionSeries)
        deleteSeries(m_initialRegressionSeries);

    if (m_pcaRegressionSeries)
        deleteSeries(m_pcaRegressionSeries);
}
