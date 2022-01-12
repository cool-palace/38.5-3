#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button, &QPushButton::clicked, [this]() {
        filepath = QFileDialog::getOpenFileName(nullptr, "Open a .jpg image", "", "jpg files (*.jpg)");
        processImage();
    });

    connect(ui->slider, QOverload<int>::of(&QAbstractSlider::valueChanged), [this]() {
        processImage();
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage MainWindow::blurImage(QImage source, int blurRadius) {
    if (source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    auto blur = QSharedPointer<QGraphicsBlurEffect>(new QGraphicsBlurEffect);
    blur.data()->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur.data());
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
    return result;
}

void MainWindow::processImage() {
    QImage sourceImage = QImage(filepath);
    ui->image->setPixmap(QPixmap::fromImage(blurImage(sourceImage, ui->slider->value()).scaled(
                                        ui->image->width(),
                                        ui->image->height(), Qt::KeepAspectRatio)));
}
