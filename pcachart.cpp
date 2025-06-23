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

    legend()->hide();
}

void PCAChart::setDataSeries(QScatterSeries *series)
{
    m_dataSeries = series;
    addAndAttachSeries(series);
}

void PCAChart::setMeanDataSeries(QScatterSeries *series)
{
    m_meanDataSeries = series;
    addAndAttachSeries(series);
}

void PCAChart::setReducedDataSeries(QScatterSeries *series)
{
    m_reducedDataSeries = series;
    addAndAttachSeries(series);
}

void PCAChart::removeDataSeries()
{
    removeSeries(m_dataSeries);
}

void PCAChart::addAndAttachSeries(QAbstractSeries *series)
{
    addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void PCAChart::setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);

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
    QPen axisPen(color);
    axisPen.setWidth(2);

    xAxisLine->setPen(axisPen);
    yAxisLine->setPen(axisPen);

    xAxisLine->setPointsVisible(false);
    yAxisLine->setPointsVisible(false);
}

void PCAChart::showDataSeries(bool show)
{
    if (!m_dataSeries) return;

    if (show)
        m_dataSeries->show();
    else
        m_dataSeries->hide();
}

void PCAChart::showMeanDataSeries(bool show)
{
    if (!m_meanDataSeries) return;

    if (show)
        m_meanDataSeries->show();
    else
        m_meanDataSeries->hide();
}

void PCAChart::showReducedDataSeries(bool show)
{
    if (!m_reducedDataSeries) return;

    if (show)
        m_reducedDataSeries->show();
    else
        m_reducedDataSeries->hide();
}

