#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "objectloader.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderer(new Renderer(this))
    , refreshTimer(new QTimer(this))
{
    ui->setupUi(this);

    connect(ui->actionfiles, &QAction::triggered, this, &MainWindow::onOpenFile);
}

MainWindow::~MainWindow()
{
    delete renderer;
    delete refreshTimer;
    delete ui;
}

void MainWindow::onOpenFile()
{
    // Ouvrir un fichier avec QFileDialog
        QString filePath = QFileDialog::getOpenFileName(this, "Open File ...", "../cute-gis/data", "GeoJSON Files All Files (*.*);; (*.geojson);;OBJ Files (*.obj)");

    if (filePath.isEmpty()) {
        qWarning() << "No file selected!";
        return;
    }

    qDebug() << "Selected File:" << filePath;

    // Charger le fichier sélectionné
    const std::string filedata = filePath.toStdString();

    renderer->reset();

    try {
        if (filePath.endsWith(".geojson", Qt::CaseInsensitive)) {
            Geojsonloader geo(filedata);
            renderer->lst_layers2d.push_back(geo);
            renderer->controller->getCamera().centerOnBoundingBox(renderer->lst_layers2d.back().boundingBox);
            renderer->setIs3D(false);
        } else if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
            ObjectLoader* objectLoader = new ObjectLoader(filedata, this);
            renderer->setObjectLoader(objectLoader);
            renderer->setIs3D(true);
        } else {
            throw std::runtime_error("Unsupported file format!");
        }
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", QString::fromStdString(ex.what()));
        return;
    }

    if (!ui->openGLWidget->layout()) {
        auto* layout = new QVBoxLayout(ui->openGLWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(renderer);
    } else {
        if (ui->openGLWidget->layout()->indexOf(renderer) == -1) {
            ui->openGLWidget->layout()->addWidget(renderer);
        }
    }

    connect(refreshTimer, &QTimer::timeout, renderer, QOverload<>::of(&QWidget::update));
    if (!refreshTimer->isActive()) {
        refreshTimer->start(16); // Rafraîchissement à ~60 FPS
    }
}
