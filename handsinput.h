#ifndef HANDSINPUT_H
#define HANDSINPUT_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QRect>
#include <QTimer>
#include "handwritinginput.h"

namespace Ui {
class handsinput;
}

class handsinput : public QMainWindow
{
    Q_OBJECT

public:
    explicit handsinput(QWidget *parent = nullptr);
    ~handsinput();
    HandwritingInput inputs;
    InputResults tempRlt;

private slots:
    void onWordInputClicked();
    void onTextInputClicked();
    void onWordButtonClicked();
    void onClearButtonClicked();  // 清空按钮的槽函数
    void onRecognitionComplete(); // 手写识别完成后的槽函数
    void onUndoButtonClicked();


protected:
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::handsinput *ui;
    QStackedWidget *stackedWidget;
    void clearButtonText(); // 清空所有按钮上的文本
    QVector<QString> regData; // 用于保存手写识别的结果
    QTimer *recognitionTimer; // 用于定时器
    bool autoinput; // 用于自动输入标志
};

#endif // HANDSINPUT_H
