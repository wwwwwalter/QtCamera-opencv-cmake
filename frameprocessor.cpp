#include "frameprocessor.h"

void FrameProcessor::run()
{
    while (true) {
        qDebug()<<m_queue.size();
        m_semaphore.acquire(); // 等待信号量变为非零，表示队列中有元素

        // QThread::msleep(100); // 延时1000毫秒



        if (m_queue.isEmpty()) {
            continue; // 如果队列在等待过程中变空，则跳过本次循环
        }

        QImage image = m_queue.dequeue(); // 从队列中取出一个图像
        // 执行耗时的图像处理操作...

        cv::Mat mat = cv::Mat(image.height(),
                              image.width(),
                              CV_8UC4,
                              (uchar *) image.constBits(),
                              image.bytesPerLine());

        // QImage rgbImage = image.convertToFormat(QImage::Format_BGR888);
        // cv::Mat mat = cv::Mat(rgbImage.height(), rgbImage.width(), CV_8UC3, (uchar*)rgbImage.constBits(), rgbImage.bytesPerLine());

        // cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC4, (uchar*)image.constBits(), image.bytesPerLine());
        // cv::imshow("camera",mat);

        // int type = mat.type();
        // int channels = mat.channels();

        // if (type == CV_8UC3) // 三通道图像
        // {
        //     std::cout << "The image is in BGR format." << std::endl;
        // }
        // else if (type == CV_8UC4) // 四通道图像
        // {
        //     std::cout << "The image is in BGRA format." << std::endl;
        // }
        // else
        // {
        //     std::cout << "The image is in an unknown format." << std::endl;
        // }

        // qDebug()<<channels;

        QImage img;
        if (mat.type() == CV_8UC3) // BGR
        {
            img = QImage((const uchar *) mat.data,
                         mat.cols,
                         mat.rows,
                         mat.step,
                         QImage::Format_RGB888);
        } else if (mat.type() == CV_8UC4) // BGRA
        {
            img = QImage((const uchar *) mat.data,
                         mat.cols,
                         mat.rows,
                         mat.step,
                         QImage::Format_RGB32);
        }

        // graphicsPixmapItem->setPixmap(QPixmap::fromImage(img));

        // 使用QSharedPointer来管理QImage的生命周期
        QSharedPointer<QImage> sharedImage(new QImage(QImage(img)));

        // 将处理后的图像发送到主线程
        // QMetaObject::invokeMethod(m_receiver,
        //                           "ProcessingFinished",
        //                           Qt::QueuedConnection,
        //                           Q_ARG(QSharedPointer<QImage>, sharedImage));

        QMetaObject::invokeMethod(m_receiver,
                                  "ProcessingFinished",
                                  Qt::QueuedConnection,
                                  Q_ARG(QImage, img));

    }
}