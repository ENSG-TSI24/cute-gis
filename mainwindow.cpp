#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "geojsonviewer.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , geoJsonViewer(new GeoJsonViewer(this)) // Initialisation du widget GeoJSON
    , refreshTimer(new QTimer(this))        // Initialisation du timer
{
    ui->setupUi(this);

    // Charger un fichier GeoJSON
    const std::string geoJsonFile = "/home/formation/Bureau/test opengl/BATIMENT.geojson";
    try {
        geoJsonViewer->loadGeoJSON(geoJsonFile);
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Erreur", QString::fromStdString(ex.what()));
        return;
    }

    // Ajouter GeoJsonViewer au layout
    if (!ui->openGLWidget->layout()) {
        auto* layout = new QVBoxLayout(ui->openGLWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(geoJsonViewer);
    } else {
        ui->openGLWidget->layout()->addWidget(geoJsonViewer);
    }

    // Configurer le timer pour rafraîchir GeoJsonViewer
    connect(refreshTimer, &QTimer::timeout, geoJsonViewer, QOverload<>::of(&QWidget::update));
    refreshTimer->start(16); // Appelle `update()` toutes les ~16 ms (60 FPS)
}

MainWindow::~MainWindow()
{
    delete ui;
}
