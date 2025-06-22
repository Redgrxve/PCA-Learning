#ifndef CUSTOMCHART_H
#define CUSTOMCHART_H

#pragma once

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class CustomChart : public QChart
{
public:
    explicit CustomChart(QGraphicsItem *parent = nullptr);

    void setMainSeries(QAbstractSeries *series);
    void removeMainSeries();
    void setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY);

    void setAxesLinesColor(const QColor &color);

private:
    QLineSeries *xAxisLine;
    QLineSeries *yAxisLine;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QAbstractSeries *m_mainSeries;
};
#endif // CUSTOMCHART_H
