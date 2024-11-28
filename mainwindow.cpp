#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderer(new Renderer(this)) // Initialisation du widget GeoJSON
    , refreshTimer(new QTimer(this))        // Initialisation du timer
{
    ui->setupUi(this);

    // Charger un fichier GeoJSON
    const std::string geoJsonFile = "../cute-gis/points_lumineux_bordeaux.geojson";
    try {
        Geojsonloader geo =  Geojsonloader(geoJsonFile);
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

MainWindow::~MainWindow()
{
    delete ui;
}
