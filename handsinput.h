﻿#ifndef HANDSINPUT_H
#define HANDSINPUT_H

#include <QMainWindow>
#include <QWidget>
#include <msinkaut.h>

namespace Ui {
class handsinput;
}

class handsinput : public QMainWindow
{
    Q_OBJECT

public:
    explicit handsinput(QMainWindow *parent = 0);
    ~handsinput();

private:
    Ui::handsinput *ui;
};

#endif // HANDSINPUT_H
