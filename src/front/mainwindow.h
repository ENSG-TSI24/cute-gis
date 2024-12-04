#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "./ui_mainwindow.h"
#include "renderer.h"
#include "geojsonloader.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GeoJsonViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_actionFlux_Data_triggered();

private:
    Ui::MainWindow *ui;
    Renderer* renderer;
    QTimer* refreshTimer;
    void onOpenFile();
    ObjectLoader *objectLoader = nullptr;

};

#endif // MAINWINDOW_H
