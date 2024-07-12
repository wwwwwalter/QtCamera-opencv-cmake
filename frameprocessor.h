#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QCoreApplication>
#include <QImage>
#include <QMutex>
#include <QPointer>
#include <QQueue>
#include <QRunnable>
#include <QSemaphore>
#include <QSharedPointer>
#include <QThread>
#include <QThreadPool>
#include <QVideoFrame>
#include <QWaitCondition>
#include <QWeakPointer>
#include "opencv2/opencv.hpp"

class FrameProcessor : public QRunnable
{
public:
    FrameProcessor(QQueue<QImage>& queue,QSemaphore& semaphore,QPointer<QObject> receiver)
        : m_queue(queue)
        , m_semaphore(semaphore)
        , m_receiver(receiver)
    {}

private:
    QQueue<QImage> &m_queue;
    QSemaphore &m_semaphore;
    QPointer<QObject> m_receiver;



    // QRunnable interface
public:
    virtual void run() override;
};

#endif // FRAMEPROCESSOR_H
