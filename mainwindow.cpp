#include "mainwindow.h"

#include <QHBoxLayout>
#include <QThreadPool>
#include "framehandle.h"
#include "frameprocessor.h"
#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // qDebug()<<geometry();
    // qDebug()<<frameGeometry();

    //main widget
    QWidget *w = new QWidget;
    setCentralWidget(w);
    QHBoxLayout *hboxlayout_main = new QHBoxLayout;
    hboxlayout_main->setSpacing(0);
    hboxlayout_main->setContentsMargins(0,0,0,0);
    w->setLayout(hboxlayout_main);

    //media layout
    videoWidget = new QVideoWidget;
    videoSink = new QVideoSink;
    graphicsView = new QGraphicsView;
    graphicsScene = new QGraphicsScene;
    graphicsPixmapItem = new QGraphicsPixmapItem;
    graphicsScene->addItem(graphicsPixmapItem);
    graphicsView->setScene(graphicsScene);
    empty_label = new QLabel(tr("no camera"));
    empty_label->setAlignment(Qt::AlignCenter);
    empty_label->setStyleSheet("QLabel { background-color : black; }");
    stackedWidget = new QStackedWidget;
    stackedWidget->insertWidget(0, graphicsView);
    stackedWidget->insertWidget(1, empty_label);
    hboxlayout_main->addWidget(stackedWidget);
    stackedWidget->setCurrentWidget(stackedWidget);


    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    resize(1920, 1080); // 在窗口显示时设置大小







    //devices
    mediaDevices = new QMediaDevices(this);
    connect(mediaDevices, &QMediaDevices::audioInputsChanged, this, &MainWindow::UpdateAudioInputDevices);
    connect(mediaDevices, &QMediaDevices::audioOutputsChanged, this, &MainWindow::UpdateAudioOutputDevices);
    connect(mediaDevices, &QMediaDevices::videoInputsChanged, this, &MainWindow::UpdateVideoInputDevices);


    //session
    session = new QMediaCaptureSession(this);

    //camera
    camera = new QCamera;//default cameraDevice
    session->setCamera(camera);
    connect(camera, &QCamera::errorOccurred, this, &MainWindow::CameraErrorOccurred);
    connect(camera, &QCamera::activeChanged, this, &MainWindow::CameraActiveChanged);
    connect(camera, &QCamera::cameraDeviceChanged, this, &MainWindow::CameraDeviceChanged);

    //output stream
    // session->setVideoOutput(videoWidget);
    session->setVideoSink(videoSink);
    audioOutput = new QAudioOutput(QMediaDevices::defaultAudioOutput());
    session->setAudioOutput(audioOutput);
    imageCapture = new QImageCapture;
    session->setImageCapture(imageCapture);
    mediaRecored = new QMediaRecorder;
    session->setRecorder(mediaRecored);
    connect(videoSink,&QVideoSink::videoFrameChanged,this,&MainWindow::VideoSinkFrameChanged);


    //input stream
    audioInput = new QAudioInput(mediaDevices->defaultAudioInput());
    session->setAudioInput(audioInput);

    // get cameraDevice Format
    const QList<QCameraFormat> &formats = camera->cameraDevice().videoFormats();

    // int index = 0;
    // for (const QCameraFormat &format : formats) {
    //     qDebug() << index++;
    //     qDebug() << "Resolution:" << format.resolution();
    //     qDebug() << "Pixel Format:" << format.pixelFormat();
    //     qDebug() << "Frame Rate Range:";
    //     qDebug() << "    Min:" << format.minFrameRate();
    //     qDebug() << "    Max:" << format.maxFrameRate();
    // }

    // set cameraDevice Format
    camera->setCameraFormat(formats[1]);



    //opencv
    // cv::namedWindow("camera", cv::WINDOW_NORMAL);



    // 创建队列和信号量
    // m_imageQueue = new QQueue<QImage>;
    // m_semaphore = new QSemaphore(0);

    // 启动工作线程
    // frameProcess = new FrameProcessor(*m_imageQueue, *m_semaphore, this);
    // QThreadPool::globalInstance()->start(frameProcess);
    // QThreadPool::globalInstance()->setMaxThreadCount(30);




    // start camera
    camera->start();















}

MainWindow::~MainWindow() {}


//mediaDevices slots
void MainWindow::UpdateAudioInputDevices() {
    qDebug()<<"UpdateAudioInputDevices"<<QMediaDevices::defaultAudioInput().description();
}

void MainWindow::UpdateAudioOutputDevices() {
    qDebug()<<"UpdateAudioOutputDevices"<<QMediaDevices::defaultAudioOutput().description();
}

void MainWindow::UpdateVideoInputDevices() {

    qDebug() << "UpdateVideoInputDevices:" << camera->cameraDevice().description();
    camera->setCameraDevice(QMediaDevices::defaultVideoInput());


    qDebug() <<"camera isAvailable:" <<camera->isAvailable();
    if (camera->isAvailable()) {
        qDebug() << "start";
        camera->start();
    }
}


//camera slots
void MainWindow::CameraErrorOccurred(QCamera::Error error, const QString &errorString) {
    qDebug() << "cameraError:" << errorString;
}

void MainWindow::CameraDeviceChanged() {
    qDebug() << "cameraDevice changed:" << camera->cameraDevice().description();
}

void MainWindow::CameraActiveChanged(bool value) {
    qDebug() << "CameraActiveChanged:" << value;

}

void MainWindow::VideoSinkFrameChanged(const QVideoFrame &frame)
{
    QImage image = frame.toImage();

    QElapsedTimer timer;
    timer.start();



    // m_imageQueue->enqueue(image);
    // m_semaphore->release(); // 释放信号量，通知工作线程队列中有新元素

    QSharedPointer<QImage> sharedImage(new QImage(image));
    QThreadPool::globalInstance()->tryStart(new FrameHandle(sharedImage,this));

    // qDebug()<<QThreadPool::globalInstance()->maxThreadCount();
    // qDebug()<<QThreadPool::globalInstance()->activeThreadCount();


    double timeTaken = timer.elapsed();
    // qDebug() << "Function took" << timeTaken << "ms";

}

void MainWindow::ProcessingFinished(QSharedPointer<QImage> sharedImage)
{
    if (!sharedImage.isNull()) {
        graphicsPixmapItem->setPixmap(QPixmap::fromImage(*sharedImage));
    }
}

void MainWindow::ProcessingFinished(QImage image)
{

    // QImage deepCopiedImage = image.copy();
    QPixmap pix = QPixmap::fromImage(image);
    graphicsPixmapItem->setPixmap(pix);
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    graphicsView->fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::moveEvent(QMoveEvent *event) {
    // qDebug()<<graphicsView->frameGeometry();
}
