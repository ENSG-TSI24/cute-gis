#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "triangle.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* openGLWidget = new Triangle(this);

    if (!ui->openGLWidget->layout()) {
        auto* layout = new QVBoxLayout(ui->openGLWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(openGLWidget);
    } else {
        ui->openGLWidget->layout()->addWidget(openGLWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
