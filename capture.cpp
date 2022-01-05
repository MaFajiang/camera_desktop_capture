#include "capture.h"

Capture::Capture()
{
    _init();
    bool bOpen = false;
#ifdef CAPTURE_DESKTOP
    bOpen = OpenGDIGrab();
#else
    bOpen = OpenCamera();
#endif
    if (bOpen)
    {
        Display();
    }
}

bool Capture::OpenGDIGrab()
{
    options = NULL;
    ifmt = av_find_input_format("gdigrab");
    if (avformat_open_input(&pFormatCtx, "desktop", ifmt, &options) != 0)
    {
        qDebug() << "Cant't open input stream";
        return false;
    }

    qDebug() << "Success to open input stream";
    return true;
}

bool Capture::OpenCamera()
{
    ifmt=av_find_input_format("dshow");
    //Set own video device's name
    if(avformat_open_input(&pFormatCtx,"video=USB Camera",ifmt,NULL)!=0){
        qDebug() << "Cant't open camera";
        return false;
    }

    qDebug() << "Success to open camera";
    return true;
}

void Capture::Display()
{
    this->start();
}

void Capture::run()
{
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        qDebug() << "Can't find stream information";
        return;
    }

    int nVideoIndex = -1;
    for (int i = 0; i<pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            nVideoIndex = i;
            break;
        }
    }

    if (nVideoIndex == -1)
    {
        qDebug() << "Don't find any stream";
        return;
    }

    AVCodecContext *pCodecCtx = pFormatCtx->streams[nVideoIndex]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL){
        qDebug() << "Codec not found";
        return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) > 0)
    {
        qDebug() << "Can't open Codec";
        return;
    }

    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();

    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    uint8_t *out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB32,
                   pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据

    int got_picture = -1;

    while (1)
    {
        if (av_read_frame(pFormatCtx, packet) < 0)
        {
            break; //这里认为视频读取完了
        }

        if (packet->stream_index == nVideoIndex) {
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

            if (ret < 0) {
                printf("decode error.\n");
                return;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                          (uint8_t const * const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                          pFrameRGB->linesize);
                //把这个RGB数据 用QImage加载
                QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                //qDebug() << tmpImg;
                updateFrame(tmpImg);

            }
        }
        av_free_packet(packet); //释放资源,否则内存会一直上升

        ///2017.8.7---lizhen
        Sleep(1); //停一停  不然放的太快了
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

void Capture::_init()
{
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    //Register Device
    avdevice_register_all();
}
