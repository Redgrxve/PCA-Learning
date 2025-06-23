#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QChartView>

class CustomChartView : public QChartView
{
    Q_OBJECT

public:
    explicit CustomChartView(QWidget *parent = nullptr);

    inline float zoomFactor()  const { return m_zoomFactor; }
    inline float currentZoom() const { return m_currentZoom; }

    void setZoom(float value);
    void zoomIn();
    void zoomOut();

protected:
    void mousePressEvent(QMouseEvent *event)   override;
    void mouseMoveEvent(QMouseEvent *event)    override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event)        override;

private:
    float m_zoomFactor  = 1.2f;
    float m_currentZoom = 1.0f;

    bool m_isDragging = false;

    QPoint m_lastMousePos;

signals:
    void zoomChanged(float value);
};

#endif // CUSTOMCHARTVIEW_H
