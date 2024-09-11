#include <widgetEx/qrcodeview.h>
#if ENABLE(QRCODE) ||1
#include <qrencode.h>
#include <cdlog.h>

//REF:https://github.com/zint/zint-gpl-only/blob/master/backend_qt4/qzint.h/cpp

namespace cdroid{

DECLARE_WIDGET(QRCodeView)

QRCodeView::QRCodeView(int w,int h):View(w,h){
    initView();
};

QRCodeView::QRCodeView(Context*ctx,const AttributeSet&attrs):View(ctx,attrs){
    initView();
}

QRCodeView::~QRCodeView(){
    
}

void QRCodeView::initView(){
    //mQRcode  = nullptr;
}

bool QRCodeView::resetSymbol(){

    return true;
}

void QRCodeView::setText(const std::string&text){
    if(mText!=text){
        mText = text;
        encode();
        invalidate(true);
    }
    float w,h;
}

void QRCodeView::setBarcodeColor(int color){
    /*if(mFgColor!=color){
        mFgColor = color;
        invalidate();
    }*/
}

int QRCodeView::getBarcodeColor()const{
    return 0;//mFgColor;
}

void  QRCodeView::setZoom(float zoom){
    mZoom = zoom;
    requestLayout();
}

float  QRCodeView::getZoom()const{
    return mZoom;
}

void  QRCodeView::onMeasure(int widthMeasureSpec, int heightMeasureSpec){
    const int widthMode  = MeasureSpec::getMode(widthMeasureSpec);
    const int heightMode = MeasureSpec::getMode(heightMeasureSpec);
    const int widthSize  = MeasureSpec::getSize(widthMeasureSpec);
    const int heightSize = MeasureSpec::getSize(heightMeasureSpec);
    int width,height;
    if(widthMode == MeasureSpec::EXACTLY){
        width = widthSize;
    }else{
        width = mQRImage->get_width()*mZoom;
    }
    if(heightMode == MeasureSpec::EXACTLY){
        height= heightSize;
    }else{
        height= mQRImage->get_width()*mZoom;
    }
    LOGD("setMeasuredDimension(%d,%d)",width,height);
    setMeasuredDimension(width, height);
}

extern "C" int QRspec_getMinimumVersion(int size, QRecLevel level);

void QRCodeView::encode(){
    const int version = 0;//QRspec_getMinimumVersion(mText.size(), QR_ECLEVEL_L);
    QRcode*mQRcode = nullptr;//QRcode_encodeString(mText.c_str(), version, QR_ECLEVEL_L, QR_MODE_8, 1);
    if(mQRcode){
        uint8_t*drq=mQRcode->data;
        uint32_t*qimg;
        mQRImage=Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32,mQRcode->width,mQRcode->width);
        uint32_t imgStride=mQRImage->get_stride();
        qimg=(uint32_t*)mQRImage->get_data();
        for(int y=0;y<mQRcode->width;y++){
            for(int x=0;x<mQRcode->width;x++){
                const int idx=y*mQRcode->width+x;
                qimg[y*imgStride+x]=0;
            }
        }
    }
}

void  QRCodeView::onDraw(Canvas&canvas){
    View::onDraw(canvas);
    const struct zint_vector_rect *rect;
    const struct zint_vector_hexagon *hex;
    const struct zint_vector_circle *circle;
    struct zint_vector_string *string;
    const RectF paintRect ={0,0,(float)getWidth(),(float)getHeight()};

    canvas.save();

    float xtr = paintRect.left;
    float ytr = paintRect.top;

    canvas.translate(xtr, ytr);
    canvas.scale(mZoom,mZoom);

    canvas.restore();
}

}/*endof namespace*/

#endif/*ENABLE(BARCODE)*/
