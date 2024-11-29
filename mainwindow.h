#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "./ui_mainwindow.h"
#include "geojsonviewer.h"

#include <QVBoxLayout>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GeoJsonViewer; // Pré-déclaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private :
    Ui::MainWindow *ui;
    GeoJsonViewer* geoJsonViewer; // Widget GeoJSON
    QTimer* refreshTimer;         // Timer pour rafraîchir l'affichage
};

#endif // MAINWINDOW_H
