#include "handsinput.h"
#include "ui_handsinput.h"

handsinput::handsinput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::handsinput)
{
    ui->setupUi(this);
}

handsinput::~handsinput()
{
    delete ui;
}
