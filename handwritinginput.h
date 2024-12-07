#ifndef HANDWRITINGINPUT_H
#define HANDWRITINGINPUT_H

#include <QWidget>
#include <msinkaut_i.c>
#include <msinkaut.h>
#include <vector>
#include <QStackedWidget>


typedef struct tag_InputResult
{
    char result[128];
} InputResult;

typedef std::vector<InputResult> InputResults;

class HandwritingInput : public QWidget
{
    Q_OBJECT
public:
    explicit HandwritingInput(QWidget *parent = nullptr);
    ~HandwritingInput();

    IInkCollector* g_pIInkCollector;
    IInkDisp* g_pIInkDisp;
    IInkRecognizerContext* g_pIInkRecoContext;

    void inputInit(HWND hwnd);
    void registerResults(InputResults &results);
    void releaseResources();
    void clear();
    void undo(); //撤销

private:
    std::vector<IInkStrokeDisp*> m_strokes;  //保存笔画集合，用于实现撤销操作；
};

#endif // HANDWRITINGINPUT_H
