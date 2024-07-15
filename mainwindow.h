#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QAudioDevice>
#include <QAudioInput>
#include <QAudioOutput>
#include <QCamera>
#include <QCameraDevice>
#include <QComboBox>
#include <QElapsedTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGroupBox>
#include <QImage>
#include <QImageCapture>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QPushButton>
#include <QQueue>
#include <QSemaphore>
#include <QStackedWidget>
#include <QVideoSink>
#include <QVideoWidget>
#include "frameprocessor.h"
#include "opencv2/opencv.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    QStackedWidget *stackedWidget;
    QLabel *empty_label;



private:
    //all devcies
    QMediaDevices *mediaDevices;


    //input
    QCameraDevice *cameraDevice;     //相机
    QCamera *camera = nullptr;       //相机输入流
    QAudioDevice *audioInputDevice;  //麦克风
    QAudioInput *audioInput;         //音频输入流

    //session
    QMediaCaptureSession *session;   //控制中心

    //ouput
    QAudioDevice *audioOutputDevice; //扬声器
    QAudioOutput *audioOutput;       //音频输出流
    QVideoWidget *videoWidget;       //相机预览
    QVideoSink *videoSink;           //视频接收器
    QGraphicsView *graphicsView;     //图片显示
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *graphicsPixmapItem;
    QImageCapture *imageCapture;     //拍照
    QMediaRecorder *mediaRecored;    //录像LL

    //format
    QCameraFormat *cameraFormat;    //相机输入格式
    QAudioFormat *audioFormat;      //音频输入输出格式

private:
    //opencv

private:
    // queue
    // QQueue<QImage> *m_imageQueue;
    // QSemaphore *m_semaphore;
    // FrameProcessor *frameProcess;



public slots:



    //mediaDeives slots
    void UpdateAudioInputDevices();

    void UpdateAudioOutputDevices();

    void UpdateVideoInputDevices();

    //camera slots
    void CameraErrorOccurred(QCamera::Error error, const QString &errorString);

    void CameraDeviceChanged();

    void CameraActiveChanged(bool value);

    //
    void VideoSinkFrameChanged(const QVideoFrame &frame);

    //ui
    void ProcessingFinished(QSharedPointer<QImage> sharedImage);
    void ProcessingFinished(QImage image);



    //
    // void UpdateRecorderState(QMediaRecorder::RecorderState state);

    // void DisplayCaptureError(int id, const QImageCapture::Error error, const QString &errorString);

    // void DisplayRecorderError();

    // void UpdateRecorederTime();

    // void ImageSaved(int id, const QString &filename);

    // void DisplayCapturedImage();

    // void DisplayViewfinder();

    // void ProcessCapturedImage(int requestId, const QImage &img);

    // void ReadyForCapture(bool ready);



public:
    template<typename Func>
    double measureExecutionTime(Func func) {
        QElapsedTimer timer;
        timer.start();

        // 调用函数
        func();

        // 停止计时器并返回经过的时间（毫秒）
        return timer.elapsed();
    }

    // QTC_TEMP
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;

    // QWidget interface
protected:
    virtual void moveEvent(QMoveEvent *event) override;
};
#endif // MAINWINDOW_H
