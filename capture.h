#ifndef CAPTURE_H
#define CAPTURE_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
}

#include <QObject>
#include <QImage>
#include <QThread>
#include <windows.h>
#include <QDebug>

class Capture : public QThread
{
    Q_OBJECT
public:
    explicit Capture();
    ~Capture(){}

    bool OpenGDIGrab();
    bool OpenCamera();
    void Display();
protected:
    void run();
private:
    void _init();

private:
    AVFormatContext *pFormatCtx;
    AVDictionary *options;
    AVInputFormat *ifmt;
signals:
    void updateFrame(QImage img);
signals:

public slots:
};

#endif // CAPTURE_H
