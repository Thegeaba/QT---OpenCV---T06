#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QtGui>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <QTimer>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame();

    void on_comboBox_filtrare_activated(int index);

    void on_slider_kernel_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    VideoCapture cap;
    QTimer timer;

    QString filterType = "normal";

    Mat rezTempImage;
    int kernelSize = 1;

};
#endif // MAINWINDOW_H
