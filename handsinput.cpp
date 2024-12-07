#include "handsinput.h"
#include "paintinputui.h"
#include "ui_handsinput.h"
#include <QDebug>

handsinput::handsinput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::handsinput),

    autoinput(false)
{
    ui->setupUi(this);
    WId hshow = ui->word_widget->winId();
    inputs.inputInit((HWND)hshow);
    recognitionTimer = new QTimer(this);


    // 获取 stackedWidget 控件
    stackedWidget = ui->stackedWidget;

    // 获取单字按钮和文本按钮
    QPushButton *wordInputButton = ui->word_input;
    QPushButton *textInputButton = ui->text_input;

    // 获取 QStackedWidget 中的两个页面
    QWidget *word_widget = stackedWidget->widget(0);
    QWidget *text_widget = stackedWidget->widget(1);

    // 给文本行手写绘制网格ui
    PaintInputUI *textWidgetPaint = new PaintInputUI(text_widget);
    textWidgetPaint->setGeometry(text_widget->rect());  // 设置绘制区域
    textWidgetPaint->setObjectName("text_widget");
    textWidgetPaint->show();  // 显示绘制内容

    // 连接两个单字和文本的手写按钮，对应stacked widget的不同子窗口
    connect(wordInputButton, &QPushButton::clicked, this, &handsinput::onWordInputClicked);
    connect(textInputButton, &QPushButton::clicked, this, &handsinput::onTextInputClicked);

    // 链接候选词按钮，后续完成按钮单击实现预测字的输入
    connect(ui->word1, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word2, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word3, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word4, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word5, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word6, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word7, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word8, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);
    connect(ui->word9, &QPushButton::clicked, this, &handsinput::onWordButtonClicked);

    // 连接清空按钮
    connect(ui->clean, &QPushButton::clicked, this, &handsinput::onClearButtonClicked);

    //撤销
    connect(ui->undo, &QPushButton::clicked, this, &handsinput::onUndoButtonClicked);

    // 初始化定时器
    recognitionTimer = new QTimer(this);
    connect(recognitionTimer, &QTimer::timeout, this, &handsinput::onRecognitionComplete);
    recognitionTimer->setSingleShot(true); // 只触发一次
}

handsinput::~handsinput()
{
    recognitionTimer->stop();
    delete ui;
}

// 单字按钮点击时的槽函数
void handsinput::onWordInputClicked()
{
    // 切换到 word_widget 页面
    stackedWidget->setCurrentWidget(ui->word_widget);
}

// 文本按钮点击时的槽函数
void handsinput::onTextInputClicked()
{
    // 切换到 text_widget 页面
    stackedWidget->setCurrentWidget(ui->text_widget);
}

// 词按钮点击时的槽函数
void handsinput::onWordButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        QString word = button->text();
        qDebug() << "Clicked on: " << word;
    }
}

// 清空所有按钮上的文本
void handsinput::clearButtonText()
{
    ui->word1->setText("");
    ui->word2->setText("");
    ui->word3->setText("");
    ui->word4->setText("");
    ui->word5->setText("");
    ui->word6->setText("");
    ui->word7->setText("");
    ui->word8->setText("");
    ui->word9->setText("");
}


// 清空按钮点击时的槽函数
void handsinput::onClearButtonClicked()
{
    clearButtonText();  // 清空所有按钮上的文本
    regData.clear();    // 清空识别结果数据
    inputs.clear();
    ui->stackedWidget->update();
}

void handsinput::onUndoButtonClicked()
{
   qDebug() <<"Undo button was clicked!";
}


// 手写识别完成后的槽函数
void handsinput::onRecognitionComplete()
{
    if (!regData.isEmpty())
    {
        // 将识别结果显示在 word1 到 word9 按钮上
        for (int i = 0; i < regData.size() && i < 9; ++i)
        {
            QPushButton *wordButton = findChild<QPushButton *>("word" + QString::number(i + 1));
            if (wordButton)
            {
                wordButton->setText(regData[i]);
            }
        }
    }
}
void handsinput::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);  // 确保调用基类的处理
    autoinput = false;
    recognitionTimer->start(2000);  // 开始定时器
    tempRlt.clear();
    inputs.registerResults(tempRlt);  // 进行识别
    // 更新按钮显示识别结果
    ui->word1->setText(QString::fromUtf8(QByteArray(tempRlt[0].result)));
    ui->word2->setText(QString::fromUtf8(QByteArray(tempRlt[1].result)));
    ui->word3->setText(QString::fromUtf8(QByteArray(tempRlt[2].result)));
    ui->word4->setText(QString::fromUtf8(QByteArray(tempRlt[3].result)));
    ui->word5->setText(QString::fromUtf8(QByteArray(tempRlt[4].result)));
    ui->word6->setText(QString::fromUtf8(QByteArray(tempRlt[5].result)));
    ui->word7->setText(QString::fromUtf8(QByteArray(tempRlt[6].result)));
    ui->word8->setText(QString::fromUtf8(QByteArray(tempRlt[7].result)));
    ui->word9->setText(QString::fromUtf8(QByteArray(tempRlt[8].result)));
}
