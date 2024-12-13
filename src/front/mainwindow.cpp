#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "objectloader.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QListWidget>
#include <QInputDialog>
#include "../back/vectordata.h"
#include "../back/rasterdata.h"
#include "addFluxData.h"
#include "renderer.h"
#include "renderer2d.h"
#include "renderer3d.h"

#include <QTableWidget>
#include <QTableWidgetItem>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QLabel>
#include <ogrsf_frmts.h>


#include "../back/API_WFS.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderer(new Renderer(this))
    , refreshTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->actionfiles, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->button_3d, &QPushButton::clicked, this, &MainWindow::onToggle3DMode);
//    connect(this, &MainWindow::geometrySelected, renderer->getRenderer2d(), &Renderer2D::highlightGeometry);

}

MainWindow::~MainWindow()
{
    delete renderer;
    delete refreshTimer;
    delete ui;
}

void MainWindow::onToggle3DMode()
{
    bool isCurrently3D = renderer->getIs3D();
    renderer->setIs3D(!isCurrently3D);

    if (!isCurrently3D) {
        renderer->reset2D();
        name_layers.clear();
        setupCheckboxes();
        QMessageBox::information(this, "mode Changed", "3D mode activated");
    } else {
        renderer->reset3D();
        QMessageBox::information(this, "Mode Changed", "2D mode activated");
    }

    ui->button_3d->setText(!isCurrently3D ? "Switch to 2D Mode" : "Switch to 3D Mode");
}

void MainWindow::onOpenFile()
{
    bool is3DMode = renderer->getIs3D();

    QString filter = is3DMode ? "3D Files (*.obj);;All Files (*.*)" : "2D Files (*.geojson *.shp);;All Files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(this, "Open File ...", "../cute-gis/data", filter);

    if (filePath.isEmpty()) {
        qWarning() << "No file selected!";
        return;
    }

    qDebug() << "Selected File:" << filePath;
    std::string filestr = filePath.toStdString();
    const char* filedata = filestr.c_str();
    renderer->reset3D();

    try {
        if (filePath.endsWith(".geojson", Qt::CaseInsensitive) || filePath.endsWith(".shp", Qt::CaseInsensitive)) {
            renderer->reset3D();
            //add layer2d
            std::cout<<"############### ADD LAYER ################"<<std::endl;

            VectorData geo(filedata);
            std::shared_ptr<Layer2d> vector = std::make_unique<Layer2d>(geo);
            renderer->getRenderer2d()->lst_layers2d.push_back(vector);

            // Add name layers
            QFileInfo fileInfo(filePath);
            std::string name = fileInfo.baseName().toStdString();
            renderer->getRenderer2d()->lst_layers2d.back()->setName(name);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;

            renderer->controller->getCamera().centerOnBoundingBox(renderer->getRenderer2d()->lst_layers2d.back()->getBoundingBox());
            renderer->setIs3D(false);

        } else if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
            renderer->reset2D();
            nb_layers=0;
            ObjectLoader* objectLoader = new ObjectLoader(filedata, this);
            renderer->getRenderer3d()->setObjectLoader(objectLoader);
            renderer->setIs3D(true);
        } else if (filePath.endsWith(".tif", Qt::CaseInsensitive) || filePath.endsWith(".tiff", Qt::CaseInsensitive)) {
            renderer->reset3D();
            RasterData geo = RasterData(filedata);
            std::shared_ptr<LayerRaster> raster = std::make_unique<LayerRaster>(geo);
            renderer->getRenderer2d()->lst_layers2d.push_back(raster);

            // add name layers
            QFileInfo fileInfo(filePath);
            std::string name = fileInfo.baseName().toStdString();
            renderer->getRenderer2d()->lst_layers2d.back()->setName(name);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;

            renderer->controller->getCamera().centerOnBoundingBox(renderer->getRenderer2d()->lst_layers2d.back()->getBoundingBox());
            renderer->setIs3D(false);
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
    } else if (ui->openGLWidget->layout()->indexOf(renderer) == -1) {
        ui->openGLWidget->layout()->addWidget(renderer);
    }

    connect(refreshTimer, &QTimer::timeout, renderer, QOverload<>::of(&QWidget::update));
    if (!refreshTimer->isActive()) {
        refreshTimer->start(16); // Refresh at ~60 FPS
    }
}

void MainWindow::onOpenFile_stream(const char* chemin)
{
    bool is3DMode = renderer->getIs3D();

    QString filter = is3DMode ? "OBJ Files (*.obj);;All Files (*.*)" : "GeoJSON Files (*.geojson);;GeoTIFF Files (*.tif *.tiff);;Shapefile Files (*.shp);;All Files (*.*)";
    //QString filePath = QFileDialog::getOpenFileName(this, "Open File ...", "../cute-gis/data", filter);
    QString stream_path = chemin;
    if (stream_path.isEmpty()) {
        qWarning() << "No stream dataset !";
        return;
    }

    qDebug() << "Selected File:" <<stream_path;
    std::string filestr = stream_path.toStdString();
    const char* filedata = filestr.c_str();
    renderer->reset();

    try {
        if (stream_path.endsWith(".geojson", Qt::CaseInsensitive)) {
            renderer->reset();
            //add layer2d
            std::cout<<"############### ADD LAYER ################"<<std::endl;


            VectorData geo(filedata);
            std::shared_ptr<Layer2d> vector = std::make_unique<Layer2d>(geo);
            renderer->getRenderer2d()->lst_layers2d.push_back(vector);

            // Add name layers
            QFileInfo fileInfo(stream_path);
            std::string name = fileInfo.baseName().toStdString();
            renderer->getRenderer2d()->lst_layers2d.back()->setName(name);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;

            renderer->controller->getCamera().centerOnBoundingBox(renderer->getRenderer2d()->lst_layers2d.back()->getBoundingBox());
            renderer->setIs3D(false);

        } else if (stream_path.endsWith(".obj", Qt::CaseInsensitive)) {
            renderer->reset2D();
            nb_layers=0;
            ObjectLoader* objectLoader = new ObjectLoader(filedata, this);
            renderer->getRenderer3d()->setObjectLoader(objectLoader);
            renderer->setIs3D(true);
        } else if (stream_path.endsWith(".tif", Qt::CaseInsensitive) || stream_path.endsWith(".tiff", Qt::CaseInsensitive)) {
            renderer->reset3D();
            RasterData geo = RasterData(filedata);
            std::shared_ptr<LayerRaster> raster = std::make_unique<LayerRaster>(geo);
            renderer->getRenderer2d()->lst_layers2d.push_back(raster);

            // add name layers
            QFileInfo fileInfo(stream_path);
            std::string name = fileInfo.baseName().toStdString();
            renderer->getRenderer2d()->lst_layers2d.back()->setName(name);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;

            renderer->controller->getCamera().centerOnBoundingBox(renderer->getRenderer2d()->lst_layers2d.back()->getBoundingBox());
            renderer->setIs3D(false);
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
    } else if (ui->openGLWidget->layout()->indexOf(renderer) == -1) {
        ui->openGLWidget->layout()->addWidget(renderer);
    }

    connect(refreshTimer, &QTimer::timeout, renderer, QOverload<>::of(&QWidget::update));
    if (!refreshTimer->isActive()) {
        refreshTimer->start(16); // Refresh at ~60 FPS
    }
}



void MainWindow::on_actionFlux_Data_triggered() {
    addFluxData dialog(this);  // Create the dialog instance
    if (dialog.exec() == QDialog::Accepted) {  // Wait for user interaction
        QString layerName = dialog.getLayerName();  // Get the layer name
        QString layerURL = dialog.getLayerURL();    // Get the layer URL

        qDebug() << "Layer Name:" << layerName;
        qDebug() << "URL:" << layerURL;
     std::string  intername =layerName.toStdString();
        const char* cclayerName = intername.c_str();

        // intername.c_str();
        std::string  intername2 =layerURL.toStdString();
        const char* wfsUrl = intername2.c_str();
        std::cout << wfsUrl << std::endl;

        std::cout <<"oui 1" <<std::endl;
        API_WFS wfs(wfsUrl);
        try {
            wfs.loadDataset();
            if (!wfs.isEmpty()) {
            char** layers = wfs.displayMetadata(); // Get the list of layers

                    for (int i = 0; layers[i] != nullptr; ++i) { // Iterate until null terminator
                        std::cout << "Layer " << i + 1 << ": " << layers[i] << std::endl;
                    }
            std::cout <<"I'm here"<<std::endl ;
            wfs.ExportToGeoJSON(intername);
            std::vector<std::pair<std::string, std::string>> FieldsLayers = wfs.GetLayerFields(cclayerName);
            for (const auto& field : FieldsLayers) {
                   std::cout << "Field: " << field.first << ", Type: " << field.second << std::endl;
               }
            const char* chemin = wfs.getOutput();
            onOpenFile_stream(chemin);


    }
        }  catch (const std::exception& e) {
            std::cout << "An error occurred: " << e.what() << std::endl; // **frontend team ( error log )
            on_actionFlux_Data_triggered();
        }



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

    for (auto& layer : renderer->getRenderer2d()->lst_layers2d) {
        if (layer->getName() == name) {
            layer->setIsVisible(checked);
        }
    }
}

void MainWindow::onLayerContextMenuRequested(const QPoint& pos) {
    QListWidget* listWidget = qobject_cast<QListWidget*>(sender());
    if (!listWidget) return;

    QListWidgetItem* item = listWidget->itemAt(pos);
    if (!item) return;

    QMenu contextMenu(this);

    QAction* zoomLayer = contextMenu.addAction("Focus");
    QAction* renameAction = contextMenu.addAction("Rename");
    QAction* deleteAction = contextMenu.addAction("Delete");
    QAction* metadataAction = contextMenu.addAction("Attribute table");
    QAction* opacityAction = contextMenu.addAction("Opacity");

    QAction* selectedAction = contextMenu.exec(listWidget->mapToGlobal(pos));

    int row = listWidget->row(item);
        if (row < 0 || row >= static_cast<int>(renderer->getRenderer2d()->lst_layers2d.size())) return;

    if (selectedAction == renameAction) {
        bool ok;
        QString newName = QInputDialog::getText(this, "Rename Layer", "New name:", QLineEdit::Normal, item->text(), &ok);
        if (ok && !newName.isEmpty()) {
            item->setText(newName);
            name_layers[listWidget->row(item)] = newName.toStdString();
        }
    } else if (selectedAction == deleteAction) {
        int row = listWidget->row(item);
        delete listWidget->takeItem(row);
        name_layers.erase(name_layers.begin() + row);
        renderer->getRenderer2d()->lst_layers2d.erase(renderer->getRenderer2d()->lst_layers2d.begin() + row);
    } else if  (selectedAction == zoomLayer) {
        auto& layer = renderer->getRenderer2d()->lst_layers2d[row];
        renderer->controller->getCamera().centerOnBoundingBox(layer->getBoundingBox());

        // falcultative
        QMessageBox::information(this, "Zoom", "Zoomed to layer: " + QString::fromStdString(layer->getName()));
    } else if (selectedAction == metadataAction) {
        const auto& layer = renderer->getRenderer2d()->lst_layers2d[row];
        showAttributeTable(layer);
    } else if (selectedAction == opacityAction) {
        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("Adjust Layer Opacity");
        dialog->resize(300, 100);
        QVBoxLayout* layout = new QVBoxLayout(dialog);
        QLabel* label = new QLabel("Opacity (0% - 100%):", dialog);
        layout->addWidget(label);
        QSlider* slider = new QSlider(Qt::Horizontal, dialog);
        slider->setRange(0, 100);
        double currentOpacity = renderer->getRenderer2d()->lst_layers2d[row]->getOpacity();
        slider->setValue(static_cast<int>(currentOpacity * 100));
        layout->addWidget(slider);

        QLabel* valueLabel = new QLabel(QString::number(slider->value()) + "%", dialog);
        layout->addWidget(valueLabel);
        connect(slider, &QSlider::valueChanged, [this, row, valueLabel](int value) {
            valueLabel->setText(QString::number(value) + "%");
            if (row >= 0 && row < renderer->getRenderer2d()->lst_layers2d.size()) {
                renderer->getRenderer2d()->lst_layers2d[row]->setOpacity(value / 100.0);
                renderer->update();
            }
        });
        dialog->setLayout(layout);
        dialog->exec();
    }

}




void MainWindow::onLayersSuperposed(const QModelIndex&, int start, int end, const QModelIndex&, int destinationRow) {
    auto& layer = renderer->getRenderer2d()->lst_layers2d[start];
    renderer->getRenderer2d()->lst_layers2d.erase(renderer->getRenderer2d()->lst_layers2d.begin() + start);

    int adjustedDestination = (destinationRow > start) ? destinationRow - 1 : destinationRow;
    renderer->getRenderer2d()->lst_layers2d.insert(renderer->getRenderer2d()->lst_layers2d.begin() + adjustedDestination, layer);
    auto name = name_layers[start];
    name_layers.erase(name_layers.begin() + start);
    name_layers.insert(name_layers.begin() + adjustedDestination, name);

    renderer->update();
}

void MainWindow::showAttributeTable(const std::shared_ptr<LayerBase>& layer) {
    // Créer une fenêtre pour afficher la table attributaire
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(QString::fromStdString("Attributs : " + layer->getName()));
    dialog->resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Créer un QTableWidget pour afficher les attributs
    QTableWidget* tableWidget = new QTableWidget(dialog);
    tableWidget->setColumnCount(layer->getAttributes().empty() ? 0 : layer->getAttributes()[0].size());
    tableWidget->setRowCount(layer->getAttributes().size());

    // Définir les en-têtes des colonnes
    QStringList headers;
    for (const auto& header : layer->getAttributeHeaders()) {
        headers << QString::fromStdString(header);
    }
    tableWidget->setHorizontalHeaderLabels(headers);

    // Remplir les données de la table
    for (int i = 0; i < layer->getAttributes().size(); ++i) {
        const auto& row = layer->getAttributes()[i];
        for (int j = 0; j < row.size(); ++j) {
            tableWidget->setItem(i, j, new QTableWidgetItem(QString::fromStdString(row[j])));
        }
    }

    tableWidget->resizeColumnsToContents();
    layout->addWidget(tableWidget);

    dialog->setLayout(layout);

    // Connect the table's selection signal to directly call the highlightGeometry method
    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, [this, &layer, tableWidget]() {
        QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            renderer->getRenderer2d()->highlightGeometry(layer->getName(), row);

            // Trigger a refresh of the OpenGL widget
            this->update(); // Or call the appropriate method to refresh the rendering
        }
    });


    dialog->exec(); // Afficher la boîte de dialogue
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
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onLayerContextMenuRequested);
    connect(listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::onLayersSuperposed);


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
    listWidget->setStyleSheet("QListWidget::item { padding: 5px; font-family: Sans Serif; font-size: 10pt; }");

    // setting police size
    QFont font = listWidget->font();
    font.setPointSize(15);
    listWidget->setFont(font);

    // Set layer_manager main layout
    auto* layout = new QVBoxLayout(ui->layer_manager);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(listWidget);
    layout->addStretch();

    ui->layer_manager->setLayout(layout);

}
