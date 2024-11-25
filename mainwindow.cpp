#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int width = 900;
    int lenght = 900;

    resize(width, lenght);
    setWindowTitle(tr("CuteGis Mainpage"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

