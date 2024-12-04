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

            //add layer2d
            Geojsonloader geo(filedata);
            renderer->lst_layers2d.push_back(geo);

            // add name layers
            std::string name = "Couche " + std::to_string(nb_layers);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;

            renderer->controller->getCamera().centerOnBoundingBox(renderer->lst_layers2d.back().boundingBox);

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

void MainWindow::clearLayout(QLayout *layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}


void MainWindow::setupCheckboxes(){

    if (ui->layer_manager->layout()) {
        clearLayout(ui->layer_manager->layout());
        delete ui->layer_manager->layout();
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->layer_manager);

    layout->setSizeConstraint(QLayout::SetMaximumSize);

    layout->setContentsMargins(10, 10, 10, 10);

    for (const auto& name : name_layers) {
        QCheckBox *checkbox = new QCheckBox(QString::fromStdString(name), ui->layer_manager);

        checkbox->setStyleSheet("QCheckBox::indicator { width: 16px; height: 16px; }"
                                "QCheckBox { padding-left: 5px; "
                                "            font-family: Sans Serif;"
                                "            font-size:    12pt; "
                                "            font-weight: normal;}"
                                );

        checkbox->setFixedHeight(20);

        layout->addWidget(checkbox);
        layout->addSpacing(10);
    }
    layout->addStretch();

    ui->layer_manager->setLayout(layout);
}
