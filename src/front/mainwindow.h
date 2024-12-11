#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "./ui_mainwindow.h"
#include "renderer.h"
#include "objectloader.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearLayout(QLayout *layout);
    void onCheckboxToggled(bool checked, std::string name);
    void onToggle3DMode();

private slots:
    void on_actionFlux_Data_triggered();

private:
    Ui::MainWindow *ui;
    void setupCheckboxes();
    Renderer* renderer;
    QTimer* refreshTimer;
    void onOpenFile();
    void onOpenFile_stream(const char* chemin);
    ObjectLoader *objectLoader = nullptr;
    std::vector<std::string> name_layers;
    int nb_layers = 0;
    void onLayerContextMenuRequested(const QPoint& pos);

};

#endif // MAINWINDOW_H
