#ifndef PCACHART_H
#define PCACHART_H

#pragma once

#include <QtCharts/QChart>

class QLineSeries;
class QScatterSeries;
class QValueAxis;

class PCAChart : public QChart
{
public:
    explicit PCAChart(QGraphicsItem *parent = nullptr);

    QScatterSeries *dataSeries()        const { return m_dataSeries; }
    QScatterSeries *meanDataSeries()    const { return m_meanDataSeries; }
    QScatterSeries *reducedDataSeries() const { return m_reducedDataSeries; }

    void setDataSeries(QScatterSeries *series);
    void setMeanDataSeries(QScatterSeries *series);
    void setReducedDataSeries(QScatterSeries *series);

    void setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void setAxesLinesColor(const QColor &color);

    void showDataSeries(bool show = true);
    void showMeanDataSeries(bool show = true);
    void showReducedDataSeries(bool show = true);

    void removeDataSeries();

private:
    void addAndAttachSeries(QAbstractSeries *series);

    QLineSeries *xAxisLine;
    QLineSeries *yAxisLine;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QScatterSeries *m_dataSeries;
    QScatterSeries *m_meanDataSeries;
    QScatterSeries *m_reducedDataSeries;
};
#endif // CUSTOMCHART_H
