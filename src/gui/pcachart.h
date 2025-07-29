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

    inline void setTrainDataSeries(QScatterSeries *series) { m_trainDataSeries = series; }
    inline void setTestDataSeries(QScatterSeries *series)  { m_testDataSeries = series; }

    void setAxesTitles(const QString &x, const QString &y);
    void setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void setAxesLinesColor(const QColor &color);

    void showTrainDataSeries(bool show = true);
    void showTestDataSeries(bool show = true);

    void showAxesLines(bool show = true);

    void clearAllDataSeries();
    void clearInitialDataSeries();
    void clearReducedDataSeries();
    void clearPCADataSeries();

    void adjustAxesRange();

    void addAndAttachSeries(QAbstractSeries *series);
    void addAndAttachSeries(QScatterSeries *series);
    void addAndAttachSeries(QLineSeries *series);

private:
    void showSeries(QAbstractSeries *series, bool show = true);

    template<typename T>
    void deleteSeries(T* &series) {
        removeSeries(series);
        delete series;
        series = nullptr;
    }

    QLineSeries *xAxisLine{};
    QLineSeries *yAxisLine{};

    QValueAxis *axisX{};
    QValueAxis *axisY{};

    QScatterSeries *m_trainDataSeries{};
    QScatterSeries *m_testDataSeries{};

    QColor m_axesLinesColor = Qt::black;
    int  m_axesLinesWith = 1;
    int m_axesXTickCount = 25;
    int m_axesYTickCount = 25;

    qreal m_scatterSeriesMarkerSize = 10.0;

    QColor m_lineSeriesColor = Qt::red;
    int m_lineSeriesWidth = 2;
};
#endif // CUSTOMCHART_H
