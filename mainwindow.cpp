#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap.open(0);
    if (!cap.isOpened()) {
        qDebug() << "ERROR: Camera nu a putut fi deschisa";
        close();
    }
    connect(&timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer.start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFrame()
{
    Mat frame;
    cap >> frame;

    if (frame.empty()) {
        qDebug() << "ERROR: Captura frame-ului nu a reusit";
        return;
    }

    cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    Mat rezTempImage = frame;

    if (filterType == "blur") {
        blur(frame, rezTempImage, Size(kernelSize, kernelSize));
    } else if (filterType == "gaussianBlur"){
        GaussianBlur(frame, rezTempImage, Size(kernelSize, kernelSize), 0);
    }
    else if (filterType == "sobelVertical"){
        Mat grayscaleImage;
        cvtColor(frame, grayscaleImage, cv::COLOR_BGR2GRAY);
        Mat sobelResult;
        Sobel(grayscaleImage, sobelResult, CV_8U, 0, 1, kernelSize, 0.4, 128);
        cvtColor(sobelResult, rezTempImage, cv::COLOR_GRAY2RGB);
    }
    else if (filterType == "sobelHorizontal"){
        Mat grayscaleImage;
        cvtColor(frame, grayscaleImage, cv::COLOR_BGR2GRAY);
        Mat sobelResult;
        Sobel(grayscaleImage, sobelResult, CV_8U, 1, 0, kernelSize, 0.4, 128);
        cvtColor(sobelResult, rezTempImage, cv::COLOR_GRAY2RGB);
    }
    else if (filterType == "medianFilter"){
        medianBlur(frame, rezTempImage, kernelSize);
    }
    else if (filterType == "bilateralFilter"){
        Mat rezTempImage;
        bilateralFilter(frame, rezTempImage, 4, 200, 80);
    }

    QImage tempImage(rezTempImage.data, rezTempImage.cols, rezTempImage.rows, rezTempImage.step, QImage::Format_RGB888);
    tempImage = tempImage.scaled(ui->label_webcam->size(), Qt::KeepAspectRatio);
    ui->label_webcam->setPixmap(QPixmap::fromImage(tempImage));
}

// Alege Filtru
void MainWindow::on_comboBox_filtrare_activated(int index)
{
    switch(index){
    // Normal
    case 0:
        filterType = "normal";
        break;
    // Blur
    case 1:
        filterType = "blur";
        break;
    // Gaussian Blur
    case 2:
        filterType = "gaussianBlur";
        break;
    // Sobel Vertical
    case 3:
        filterType = "sobelVertical";
        break;
    // Sobel Orizontal
    case 4:
        filterType = "sobelHorizontal";
        break;
    // Filtrul Median
    case 5:
        filterType = "medianFilter";
        break;
    // Filtrul Bilateral
    case 6:
        filterType = "bilateralFilter";
        break;
    }
}

// Kernel Size
void MainWindow::on_slider_kernel_valueChanged(int value)
{
    if(value % 2 == 0){
        kernelSize = value + 1;
    }
    else{
        kernelSize = value;
    }

    QString labelKernelSize = "Kernel: " + QString::number(kernelSize);
    ui->label_kernelSize->setText(labelKernelSize);
}

