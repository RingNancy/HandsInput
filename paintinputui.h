#ifndef PAINTINPUTUI_H
#define PAINTINPUTUI_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "handsinput.h"
#include "handwritinginput.h"

class PaintInputUI : public QWidget
{
    Q_OBJECT

public:
    explicit PaintInputUI(QWidget *parent = nullptr);
    ~PaintInputUI();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawGrid(QPainter &painter);
};

#endif // PAINTINPUTUI_H
