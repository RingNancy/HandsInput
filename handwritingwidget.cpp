#include "HandWritingWidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QPaintEvent>

HandWritingWidget::HandWritingWidget(QWidget *parent)
    : QWidget(parent), isDrawing(false)
{
    setAutoFillBackground(true);  // 确保背景可见
}

HandWritingWidget::~HandWritingWidget() {}

void HandWritingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));  // 设置墨迹的颜色和样式

    // 绘制所有的笔画
    for (const auto &stroke : strokes) {
        for (int i = 1; i < stroke.size(); ++i) {
            painter.drawLine(stroke[i - 1], stroke[i]);
        }
    }

    // 绘制当前正在画的笔画
    if (isDrawing && !currentStroke.isEmpty()) {
        for (int i = 1; i < currentStroke.size(); ++i) {
            painter.drawLine(currentStroke[i - 1], currentStroke[i]);
        }
    }
}

void HandWritingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        currentStroke.clear();
        currentStroke.append(event->pos());
    }
}

void HandWritingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing) {
        currentStroke.append(event->pos());
        update();  // 更新窗口以重新绘制
    }
}

void HandWritingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (isDrawing) {
        isDrawing = false;
        strokes.append(currentStroke);
        update();  // 更新窗口以重新绘制
    }
}
