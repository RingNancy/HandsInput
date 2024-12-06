#include "HandWritingWidget.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QBrush>
#include <QMouseEvent>
#include <QPaintEvent>
#include <msinkaut.h>

HandWritingWidget::HandWritingWidget(QWidget *parent)
    : QWidget(parent), isDrawing(false), inkCollector(nullptr), inkDisp(nullptr), inkRecognizerContext(nullptr)
{
    setAutoFillBackground(true);  // 确保背景可见
    InitializeInkCollector(); //初始化收集器
    // 获取按钮（word1 到 word9）
    for (int i = 0; i < 9; ++i) {
        QPushButton *button = findChild<QPushButton *>(QString("word%1").arg(i + 1));
        if (button) {
            wordButtons.append(button);
        }
    }
}


HandWritingWidget::~HandWritingWidget() {
    if(inkCollector)
    {
        inkCollector->Release();
    }
    if(inkRecognizerContext)
    {
        inkRecognizerContext->Release();
    }
    if(inkDisp)
    {
        inkDisp->Release();
    }

}

void HandWritingWidget::InitializeInkCollector()
{
    //初始化 Inkcollector
    HRESULT hr = CoInitialize(NULL);
    if(FAILED(hr))
    {
        qDebug()<< "Failed to initialize COM library";
        return;
    }

    hr = CoCreateInstance(CLSID_InkCollector,NULL, CLSCTX_INPROC_SERVER, IID_IInkCollector, (void**)&inkCollector);
    if(FAILED(hr))
    {
        qDebug()<< "Failed to create InkCollector instance";
        return;
    }


    inkCollector->put_hWnd(reinterpret_cast<OLE_HANDLE>(winId()));  // 将控件的窗口句柄传递给 InkCollector
    inkCollector->put_Enabled(TRUE);  // 启用 InkCollector

    // 创建 IInkRecognizerContext 用于识别墨迹
    hr = CoCreateInstance(CLSID_InkRecognizerContext, NULL, CLSCTX_INPROC_SERVER, IID_IInkRecognizerContext, (void**)&inkRecognizerContext);
    if (FAILED(hr)) {
        qDebug() << "Failed to create InkRecognizerContext instance";
        return;
    }

    // 创建 IInkDisp 用于存储墨迹
    hr = CoCreateInstance(CLSID_InkDisp, NULL, CLSCTX_INPROC_SERVER, IID_IInkDisp, (void**)&inkDisp);
    if (FAILED(hr)) {
        qDebug() << "Failed to create InkDisp instance";
        return;
    }

}

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

        // 将手写输入添加到 InkCollector 中
        IInkStrokeDisp *inkStroke;
        inkDisp->CreateStroke(&inkStroke);
        inkStroke->get_Ink(new InkPoint(currentStroke[0].x(), currentStroke[0].y()));  // 添加第一个点（简化实现）

        // 识别墨迹
        recognizeInk();
    }
}

void HandWritingWidget::recognizeInk()
{
    if (inkRecognizerContext && inkDisp) {
        // 将墨迹添加到识别上下文
        IInkRecognitionResult *result;
        HRESULT hr = inkRecognizerContext->Recognize(NULL, &result);
        if (SUCCEEDED(hr)) {
            // 获取识别结果并更新按钮
            BSTR resultText;
            result->get_TopString(&resultText);
            updateWordButtons(QString::fromWCharArray(resultText));
        }
    }
}

void HandWritingWidget::updateWordButtons(const QString& resultText)
{
    // 将识别结果绑定到按钮上
    for (int i = 0; i < 9; ++i) {
        if (wordButtons[i]) {
            wordButtons[i]->setText(resultText);
        }
    }
}
