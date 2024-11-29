#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "objectloader.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderer(new Renderer(this)) // Initialisation du widget GeoJSON
    , refreshTimer(new QTimer(this))        // Initialisation du timer
{
    ui->setupUi(this);

    connect(ui->actionfiles, &QAction::triggered, this, &MainWindow::onOpenFile);

//    // Charger un fichier GeoJSON
//    const std::string geoJsonFile = "/home/formation/Documents/projet/cute-gis/data/geojson/points_lumineux_bordeaux.geojson";


//    try {
//        Geojsonloader geo =  Geojsonloader(geoJsonFile);
//        renderer->setCoordinates(geo.getCoordinates());
//    } catch (const std::exception& ex) {
//        QMessageBox::critical(this, "Erreur", QString::fromStdString(ex.what()));
//        return;
//    }

//    // Ajouter GeoJsonViewer au layout
//    if (!ui->openGLWidget->layout()) {
//        auto* layout = new QVBoxLayout(ui->openGLWidget);
//        layout->setContentsMargins(0, 0, 0, 0);
//        layout->addWidget(renderer);
//    } else {
//        ui->openGLWidget->layout()->addWidget(renderer);
//    }

//    // Configurer le timer pour rafraîchir GeoJsonViewer
//    connect(refreshTimer, &QTimer::timeout, renderer, QOverload<>::of(&QWidget::update));
//    refreshTimer->start(16); // Appelle `update()` toutes les ~16 ms (60 FPS)
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Pop windoe: QFileDialog
void MainWindow::onOpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File ...", "../cute-gis/data", "GeoJSON Files All Files (*.*);; (*.geojson);;OBJ Files (*.obj)");
    if (!filePath.isEmpty()) {
        qDebug() << "Selected File:" << filePath;
    }



    // Charger un fichier GeoJSON
    const std::string filedata = filePath.toStdString();

    try {
        Geojsonloader geo =  Geojsonloader(filedata);
        renderer->setCoordinates(geo.getCoordinates());
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Erreur", QString::fromStdString(ex.what()));
        return;
    }

    // Ajouter GeoJsonViewer au layout
    if (!ui->openGLWidget->layout()) {
        auto* layout = new QVBoxLayout(ui->openGLWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(renderer);
    } else {
        ui->openGLWidget->layout()->addWidget(renderer);
    }

    // Configurer le timer pour rafraîchir GeoJsonViewer
    connect(refreshTimer, &QTimer::timeout, renderer, QOverload<>::of(&QWidget::update));
    refreshTimer->start(16); // Appelle `update()` toutes les ~16 ms (60 FPS)


}




