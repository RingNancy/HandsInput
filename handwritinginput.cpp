#include "HandwritingInput.h"
#include <QAxObject>
#include <QDebug>

HandwritingInput::HandwritingInput(QWidget *parent) : QWidget(parent)
{
}

HandwritingInput::~HandwritingInput()
{
    releaseResources();
}

void HandwritingInput::inputInit(HWND hwnd)
{
    if (NULL == hwnd)
        return;

    g_pIInkCollector = nullptr;
    g_pIInkDisp = nullptr;
    g_pIInkRecoContext = nullptr;

    // Initialize COM
    CoInitialize(NULL);
    HRESULT hr;

    // Create recognition context
    hr = CoCreateInstance(CLSID_InkRecognizerContext, NULL, CLSCTX_INPROC_SERVER, IID_IInkRecognizerContext, (void**)&g_pIInkRecoContext);
    if (FAILED(hr)) {
        qDebug() << "No handwriting recognition software installed.";
        return;
    }

    // Create ink collector object
    hr = CoCreateInstance(CLSID_InkCollector, NULL, CLSCTX_INPROC_SERVER, IID_IInkCollector, (void**)&g_pIInkCollector);
    if (FAILED(hr)) return;

    // Get ink object pointer
    hr = g_pIInkCollector->get_Ink(&g_pIInkDisp);
    if (FAILED(hr)) return;

    // Bind to HWND for handwriting input
    hr = g_pIInkCollector->put_hWnd((long)hwnd);
    if (FAILED(hr)) return;

    // Set ink drawing color
    IInkDrawingAttributes* p;
    if (SUCCEEDED(g_pIInkCollector->get_DefaultDrawingAttributes(&p))) {
        p->put_Color(RGB(255, 0, 0));

    }

    // Enable ink collection
    hr = g_pIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) return;


    //设置手写区域，
#if 1
    //设置手写只识别为一个字
    IInkRecognizerGuide *RecognizerGuide;
    hr = CoCreateInstance(CLSID_InkRecognizerGuide,NULL, CLSCTX_INPROC_SERVER,IID_IInkRecognizerGuide,(void **) &RecognizerGuide);
    if (FAILED(hr))
        return;
    InkRecoGuide recoguide;
    RECT rect;
    rect.bottom = 2;//不能为1
    rect.left = 0;
    rect.right = 2;//不能为1
    rect.top = 0;

    recoguide.rectWritingBox = rect;
    recoguide.rectDrawnBox = rect;
    recoguide.cRows = 1;//不要过大
    recoguide.cColumns = 1;
    recoguide.midline = -1;


    RecognizerGuide->put_GuideData(recoguide);
    g_pIInkRecoContext->putref_Guide(RecognizerGuide);
#endif


}

void HandwritingInput::registerResults(InputResults &results)  //手写墨迹的识别
{
    if (!results.empty()) {
        results.clear();
    }

    IInkStrokes* pIInkStrokes = nullptr;
    HRESULT hr = g_pIInkDisp->get_Strokes(&pIInkStrokes);
    if (SUCCEEDED(hr)) {
        hr = g_pIInkRecoContext->putref_Strokes(pIInkStrokes);
        if (SUCCEEDED(hr)) {
            IInkRecognitionResult* pIInkRecoResult = nullptr;
            InkRecognitionStatus recognitionStatus = IRS_NoError;

            hr = g_pIInkRecoContext->Recognize(&recognitionStatus, &pIInkRecoResult);


            if (SUCCEEDED(hr) && pIInkRecoResult != nullptr) {
                IInkRecognitionAlternates* pIInkRecoAlternates;

                hr = pIInkRecoResult->AlternatesFromSelection(0, -1, 10, &pIInkRecoAlternates);
                long count = 0;

                if (SUCCEEDED(hr) && SUCCEEDED(pIInkRecoAlternates->get_Count(&count))) {
                    IInkRecognitionAlternate* pIInkRecoAlternate = nullptr;

                    for (LONG iItem = 0; (iItem < count) && (iItem < 10); iItem++) {
                        if (SUCCEEDED(pIInkRecoAlternates->Item(iItem, &pIInkRecoAlternate))) {
                            BSTR bstr = nullptr;
                            if (SUCCEEDED(pIInkRecoAlternate->get_String(&bstr))) {
                                InputResult temp = {0};
                                QString str = QString::fromWCharArray(bstr);
                                strcpy(temp.result, str.toUtf8().data());
                                results.push_back(temp);
                            }
                            pIInkRecoAlternate->Release();
                        }
                    }
                }
            }

            g_pIInkRecoContext->putref_Strokes(nullptr);
        }
        pIInkStrokes->Release();
    }
}

void HandwritingInput::releaseResources()
{
    if (g_pIInkRecoContext != nullptr) {
        g_pIInkRecoContext->Release();
        g_pIInkRecoContext = nullptr;
    }

    if (g_pIInkDisp != nullptr) {
        g_pIInkDisp->Release();
        g_pIInkDisp = nullptr;
    }

    if (g_pIInkCollector != nullptr) {
        g_pIInkCollector->Release();
        g_pIInkCollector = nullptr;
    }

    CoUninitialize();
}

//清空墨迹
void HandwritingInput::clear()
{
    if (g_pIInkDisp != nullptr) {
        g_pIInkDisp->DeleteStrokes(0);
    }
}

//撤销上一笔墨迹；
void HandwritingInput::undo()
{

}

