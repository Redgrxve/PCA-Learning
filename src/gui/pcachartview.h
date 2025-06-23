#ifndef PCACHARTVIEW_H
#define PCACHARTVIEW_H

#include "customchartview.h"

class PCAChart;

class PCAChartView : public CustomChartView
{
public:
    explicit PCAChartView(QWidget *parent = nullptr);
    ~PCAChartView();

    inline PCAChart *pcaChart() const { return m_chart; }

private:
    PCAChart *m_chart;
};

#endif // PCACHARTVIEW_H
