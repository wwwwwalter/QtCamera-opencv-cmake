#ifndef FRAMEHANDLE_H
#define FRAMEHANDLE_H

#include <QImage>
#include <QObject>
#include <QPointer>
#include <QRunnable>
#include <QSharedPointer>

class FrameHandle : public QRunnable
{
public:
    FrameHandle(QSharedPointer<QImage> image,QPointer<QObject> receiver);
    ~FrameHandle();


    // QRunnable interface
public:
    virtual void run() override;

private:
    QSharedPointer<QImage> image;
    QPointer<QObject> m_receiver;

};

#endif // FRAMEHANDLE_H
