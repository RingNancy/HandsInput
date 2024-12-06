#ifndef HANDWRITINGWIDGET_H
#define HANDWRITINGWIDGET_H

#include "handsinput.h"
#include "ui_handsinput.h"
#include <QWidget>
#include <QPointF>
#include <QList>
#include <msinkaut.h>



class HandWritingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HandWritingWidget(QWidget *parent = nullptr);
    ~HandWritingWidget();

protected:
    void paintEvent(QPaintEvent *event) override;  // 绘制墨迹
    void mousePressEvent(QMouseEvent *event) override;  // 鼠标按下时记录起始点
    void mouseMoveEvent(QMouseEvent *event) override;  // 鼠标移动时绘制
    void mouseReleaseEvent(QMouseEvent *event) override;  // 鼠标释放时结束绘制
private:
    void InitializeInkCollector(); //收集墨迹
    void recognizeInk(); // 识别墨迹
    void updateWordButtons(const QString& resultText); // 将识别内容更新到按钮

private:
    QList<QList<QPointF>> strokes;  // 记录每一笔的轨迹
    QList<QPointF> currentStroke;   // 当前笔画的轨迹
    bool isDrawing;                 // 是否正在绘制

    IInkCollector *inkCollector;
    IInkRecognizerContext *inkRecognizerContext;
    IInkDisp *inkDisp;
    QList<QPushButton*> wordButton;
};

#endif // HANDWRITINGWIDGET_H
