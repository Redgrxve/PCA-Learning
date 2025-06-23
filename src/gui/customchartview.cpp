#include "customchartview.h"

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

CustomChartView::CustomChartView(QWidget *parent)
    : QChartView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
}

void CustomChartView::zoomIn()
{
    setZoom(m_currentZoom * m_zoomFactor);
}

void CustomChartView::zoomOut()
{
    setZoom(m_currentZoom / m_zoomFactor);
}

void CustomChartView::setZoom(float value)
{
    chart()->zoomReset();
    chart()->zoom(value);

    m_currentZoom = value;

    emit zoomChanged(m_currentZoom);
}

void CustomChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QChartView::mousePressEvent(event);
}

void CustomChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        QPointF delta = event->pos() - m_lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        m_lastMousePos = event->pos();
    }
    QChartView::mouseMoveEvent(event);
}

void CustomChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
    QChartView::mouseReleaseEvent(event);
}

void CustomChartView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        chart()->zoom(m_zoomFactor);
        m_currentZoom *= m_zoomFactor;
    }
    else {
        chart()->zoom(1 / m_zoomFactor);
        m_currentZoom /= m_zoomFactor;
    }

    emit zoomChanged(m_currentZoom);
    event->accept();
}
