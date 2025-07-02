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

    inline QScatterSeries *dataSeries()         const { return m_initialDataSeries; }
    inline QScatterSeries *reducedDataSeries()  const { return m_reducedDataSeries; }

    void setInitialDataSeries(QScatterSeries *series);
    void setReducedDataSeries(QScatterSeries *series);

    void setInitialRegressionSeries(QLineSeries *series);
    void setPCARegressionSeries(QLineSeries *series);

    void setAxesTitles(const QString &x, const QString &y);
    void setAxisRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void setAxesLinesColor(const QColor &color);

    void showInitialDataSeries(bool show = true);
    void showReducedDataSeries(bool show = true);
    void showInitialRegression(bool show = true);
    void showPCARegression(bool show = true);

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

    QScatterSeries *m_initialDataSeries{};
    QScatterSeries *m_reducedDataSeries{};

    QLineSeries *m_initialRegressionSeries{};
    QLineSeries *m_pcaRegressionSeries{};

    QColor m_axesLinesColor = Qt::black;
    int  m_axesLinesWith = 1;
    int m_axesXTickCount = 25;
    int m_axesYTickCount = 25;

    qreal m_scatterSeriesMarkerSize = 10.0;

    QColor m_lineSeriesColor = Qt::red;
    int m_lineSeriesWidth = 2;
};
#endif // CUSTOMCHART_H
