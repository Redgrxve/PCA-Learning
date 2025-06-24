#include "pcachartview.h"
#include "pcachart.h"

PCAChartView::PCAChartView(QWidget *parent)
    : CustomChartView(parent)
{
    m_chart = new PCAChart;
    m_chart->setAxisRange(0, 100, 0, 100);
    setChart(m_chart);
}

PCAChartView::~PCAChartView()
{
    delete m_chart;
}
