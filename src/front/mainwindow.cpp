#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "objectloader.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "addFluxData.h"
#include <QListWidget>

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

    try {
        if (filePath.endsWith(".geojson", Qt::CaseInsensitive)) {
            renderer->reset3D();
            //add layer2d
            std::cout<<"############### ADD LAYER ################"<<std::endl;;
            Geojsonloader geo(filedata);
            renderer->lst_layers2d.push_back(geo);


            // add name layers
            std::string name = "Couche " + std::to_string(nb_layers);
            renderer->lst_layers2d.back().name = name;
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;
            renderer->controller->getCamera().centerOnBoundingBox(renderer->lst_layers2d.back().boundingBox);
            renderer->setIs3D(false);

        } else if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
            renderer->reset2D();
            nb_layers=0;
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


void MainWindow::on_actionFlux_Data_triggered() {
    addFluxData dialog(this);  // Create the dialog instance
    if (dialog.exec() == QDialog::Accepted) {  // Wait for user interaction
        QString layerName = dialog.getLayerName();  // Get the layer name
        QString layerURL = dialog.getLayerURL();    // Get the layer URL

        qDebug() << "Layer Name:" << layerName;
        qDebug() << "URL:" << layerURL;
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

void MainWindow::onCheckboxToggled(bool checked, std::string name) {

    std::cout<<"-------------- checkbox clicked --------------"<<std::endl;
    std::cout<<checked<<std::endl;
    std::cout<<name<<std::endl;

    for (auto& layer : renderer->lst_layers2d) {
        if (layer.name == name) {
            layer.isVisible = checked;
        }
    }

}

void MainWindow::setupCheckboxes() {

    // Clear the layout_manager
    if (ui->layer_manager->layout()) {
        clearLayout(ui->layer_manager->layout());
        delete ui->layer_manager->layout();
    }

    // Create list of checkbox
    QListWidget* listWidget = new QListWidget(ui->layer_manager);
    listWidget->setDragDropMode(QAbstractItemView::InternalMove);

    for (const auto& name : name_layers) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name), listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Checked);
    }

    // Link event to checkbox
    connect(listWidget, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        bool checked = item->checkState() == Qt::Checked;
        QString name = item->text();
        onCheckboxToggled(checked, name.toStdString());
    });

    // Set checkbox style
    listWidget->setSpacing(10);
    listWidget->setMaximumWidth(300);
    listWidget->setStyleSheet("QListWidget::item { padding: 5px; font-family: Sans Serif; font-size: 12pt; }");

    // Set layer_manager main layout
    auto* layout = new QVBoxLayout(ui->layer_manager);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(listWidget);
    layout->addStretch();

    ui->layer_manager->setLayout(layout);

}

