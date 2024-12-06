#include "handsinput.h"
#include "paintinputui.h"
#include "ui_handsinput.h"
#include "handwritingwidget.h"


handsinput::handsinput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::handsinput)
{
    ui->setupUi(this);

    // 获取 stackedWidget 控件
    stackedWidget = ui->stackedWidget;

    // 获取单字按钮和文本按钮
    QPushButton *wordInputButton = ui->word_input;
    QPushButton *textInputButton = ui->text_input;

    // 获取 QStackedWidget 中的两个页面
    QWidget *word_widget = stackedWidget->widget(0);
    QWidget *text_widget = stackedWidget->widget(1);

    //给文本行手写绘制网格ui
    PaintInputUI *textWidgetPaint = new PaintInputUI(text_widget);
    textWidgetPaint->setGeometry(text_widget->rect());  // 设置绘制区域
    textWidgetPaint->setObjectName("text_widget");
    textWidgetPaint->show();  // 显示绘制内容

    //绘制笔迹
    HandWritingWidget *wordHandWriting = new HandWritingWidget(word_widget);
    wordHandWriting->setGeometry(word_widget->rect());  // 设置绘制区域为整个 word_widget
    wordHandWriting->show();  // 显示绘制控件

    // 为 text_widget 创建 HandWritingWidget 实例
    HandWritingWidget *textHandWriting = new HandWritingWidget(text_widget);
    textHandWriting->setGeometry(text_widget->rect());  // 设置绘制区域为整个 text_widget
    textHandWriting->show();  // 显示绘制控件


    // 连接按钮的点击事件
    connect(wordInputButton, &QPushButton::clicked, this, &handsinput::onWordInputClicked);
    connect(textInputButton, &QPushButton::clicked, this, &handsinput::onTextInputClicked);
}

handsinput::~handsinput()
{
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

