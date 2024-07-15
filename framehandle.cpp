#include "framehandle.h"
#include "opencv2/opencv.hpp"
#include <QDebug>

#include <QThread>

FrameHandle::FrameHandle(QSharedPointer<QImage> image,QPointer<QObject> receiver)
    : image(image)
    , m_receiver(receiver)
{}

FrameHandle::~FrameHandle() {
}

void FrameHandle::run()
{
    QImage image = *this->image;

    // QImage to cv::Mat
    cv::Mat mat = cv::Mat(image.height(),
                          image.width(),
                          CV_8UC4,
                          (uchar *) image.constBits(),
                          image.bytesPerLine());

    // cv::Mat to QImage
    // 浅拷贝
    QImage img;
    if (mat.type() == CV_8UC3) // BGR
    {
        img = QImage((const uchar *) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    } else if (mat.type() == CV_8UC4) // BGRA
    {
        img = QImage((const uchar *) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
    }

    // 模拟耗时操作
    QThread::msleep(10);


    // 深拷贝
    QImage *showImage = new QImage(img.copy());

    // 使用QSharedPointer来管理QImage的生命周期
    QSharedPointer<QImage> sharedImage(showImage);

    // 将处理后的图像发送到主线程
    QMetaObject::invokeMethod(m_receiver,
                              "ProcessingFinished",
                              Qt::QueuedConnection,
                              Q_ARG(QSharedPointer<QImage>, sharedImage));


    // QImage showImage = QImage(img.copy());

    // QMetaObject::invokeMethod(m_receiver,
    //                           "ProcessingFinished",
    //                           Qt::QueuedConnection,
    //                           Q_ARG(QImage, showImage));


    // qDebug()<<"Thread ID:"<<QThread::currentThreadId();


}
