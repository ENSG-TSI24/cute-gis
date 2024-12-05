#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "./ui_mainwindow.h"
#include "renderer.h"
#include "geojsonloader.h"

#include <QVBoxLayout>
#include <QCheckBox>
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

    void clearLayout(QLayout *layout);
    void onCheckboxToggled(bool checked, std::string name);

private:
    Ui::MainWindow *ui;
    void setupCheckboxes();
    Renderer* renderer;
    QTimer* refreshTimer;
    void onOpenFile();
    ObjectLoader *objectLoader = nullptr;
    std::vector<std::string> name_layers;
    int nb_layers = 0;

    // 新增删除层的槽函数声明
    void onDeleteLayer(size_t index); 
};

#endif // MAINWINDOW_H
