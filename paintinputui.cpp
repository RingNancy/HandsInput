#include "paintInputUI.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QWidget>

PaintInputUI::PaintInputUI(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口的背景色，确保可以看到绘制效果
    setAutoFillBackground(true);
}

PaintInputUI::~PaintInputUI()
{
    // 析构函数
}

void PaintInputUI::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 设置透明度为80%
    painter.setOpacity(0.8);
    if (this->parentWidget()->objectName() == "text_widget") {
        drawGrid(painter);  // 绘制网格
    }
}

// 绘制网格
void PaintInputUI::drawGrid(QPainter &painter)
{
    QPen pen;
    pen.setColor(Qt::gray);  // 灰色
    pen.setStyle(Qt::SolidLine);  // 实线
    painter.setPen(pen);

    // 设置十字形网格样式
    QBrush brush(Qt::CrossPattern);  // 十字网格
    painter.setBrush(brush);

    int gridSize = 20;  // 每个网格的大小

    // 绘制垂直线
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }

    // 绘制水平线
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

