#ifndef HANDSINPUT_H
#define HANDSINPUT_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>

namespace Ui {
class handsinput;
}

class handsinput : public QMainWindow
{
    Q_OBJECT

public:
    explicit handsinput(QWidget *parent = nullptr);
    ~handsinput();

private slots:
    void onWordInputClicked();
    void onTextInputClicked();
    void onWordButtonClicked();

private:
    Ui::handsinput *ui;
    QStackedWidget *stackedWidget;
};

#endif // HANDSINPUT_H
