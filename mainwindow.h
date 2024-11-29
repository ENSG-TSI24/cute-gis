#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "./ui_mainwindow.h"
#include "renderer.h"
#include "geojsonloader.h"

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

private:
    Ui::MainWindow *ui;
    Renderer* renderer; // Widget renderer
    QTimer* refreshTimer;  // Timer pour rafraîchir l'affichage
};

#endif // MAINWINDOW_H
