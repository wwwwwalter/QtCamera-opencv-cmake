#include "mainwindow.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QOpenGLWidget>
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
    view = new QGraphicsView;
    //view->setRenderHint(QPainter::Antialiasing); //开启抗锯齿
    //view->setViewport(new QOpenGLWidget()); //开启OpenGL
    scene = new QGraphicsScene;
    // scene->setSceneRect(QRectF(-960,-540,1920,1080));
    // 图片图元
    pixmapItem = new QGraphicsPixmapItem;
    // pixmapItem->setPos(0,0);
    scene->addItem(pixmapItem);


    // 文字图元
    textItem = new QGraphicsTextItem("你好\nhello");
    textItem->setFont(QFont("宋体",30));
    textItem->setFlag(QGraphicsItem::ItemIsMovable);
    textItem->setDefaultTextColor(QColor(0,255,0));
    textItem->setPos(0,0);
    scene->addItem(textItem);



    view->setScene(scene);







    empty_label = new QLabel(tr("no camera"));
    empty_label->setAlignment(Qt::AlignCenter);
    empty_label->setStyleSheet("QLabel { background-color : black; }");
    stackedWidget = new QStackedWidget;
    stackedWidget->insertWidget(0, view);
    stackedWidget->insertWidget(1, empty_label);
    hboxlayout_main->addWidget(stackedWidget);
    stackedWidget->setCurrentWidget(stackedWidget);


    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
    camera->setCameraFormat(formats[7]);



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
        pixmapItem->setPixmap(QPixmap::fromImage(*sharedImage));
    }

    // static int count = 1;
    // textItem->setPlainText(QString::number(count++));

}

void MainWindow::ProcessingFinished(QImage image)
{

    // QImage deepCopiedImage = image.copy();
    QPixmap pix = QPixmap::fromImage(image);
    pixmapItem->setPixmap(pix);
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    // view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::moveEvent(QMoveEvent *event) {
    // qDebug()<<view->frameGeometry();
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug()<<event->position().toPoint();
}
