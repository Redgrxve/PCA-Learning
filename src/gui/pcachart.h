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

    inline QScatterSeries *dataSeries()        const { return m_initialDataSeries; }
    inline QScatterSeries *centeredDataSeries()    const { return m_centeredDataSeries; }
    inline QScatterSeries *reducedDataSeries() const { return m_reducedDataSeries; }

    void setInitialDataSeries(QScatterSeries *series);
    void setCenteredDataSeries(QScatterSeries *series);
    void setReducedDataSeries(QScatterSeries *series);

    void setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void setAxesLinesColor(const QColor &color);

    void showDataSeries(bool show = true);
    void showCenteredDataSeries(bool show = true);
    void showReducedDataSeries(bool show = true);

    void clearAllDataSeries();
    void removeReducedDataSeries();
    void removeDataSeries();

private:
    void addAndAttachSeries(QAbstractSeries *series);

    QLineSeries *xAxisLine{};
    QLineSeries *yAxisLine{};
    QValueAxis *axisX{};
    QValueAxis *axisY{};

    QScatterSeries *m_initialDataSeries{};
    QScatterSeries *m_centeredDataSeries{};
    QScatterSeries *m_reducedDataSeries{};
};
#endif // CUSTOMCHART_H
