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

void PCAChart::setAxesTitles(const QString &x, const QString &y)
{
    axisX->setTitleText(x);
    axisY->setTitleText(y);
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

void PCAChart::showSeries(QAbstractSeries *series, bool show)
{
    if (show)
        series->show();
    else
        series->hide();
}

void PCAChart::showTrainDataSeries(bool show)
{
    if (!m_trainDataSeries) return;

    showSeries(m_trainDataSeries, show);
}

void PCAChart::showTestDataSeries(bool show)
{
    if (!m_testDataSeries) return;

    showSeries(m_testDataSeries, show);
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
    if (m_trainDataSeries)
        deleteSeries(m_trainDataSeries);

    if (m_testDataSeries)
        deleteSeries(m_testDataSeries);
}
