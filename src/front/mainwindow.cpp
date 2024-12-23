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


#include "../back/API_WMS.h"
#include "../back/API_WMTS.h"
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

    for (const auto& layer : renderer->getRenderer2d()->lst_layers2d) {
        QFileInfo fileInfo(QString::fromStdString(layer->getName()));
        std::string name = fileInfo.baseName().toStdString();
        layer->setName(name);

        name_layers.push_back(name);
        ++nb_layers;
    }
    setupCheckboxes();
    if (!ui->openGLWidget->layout()) {
        auto* layout = new QVBoxLayout(ui->openGLWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(renderer);
    } else {
        clearLayout(ui->openGLWidget->layout());
        ui->openGLWidget->layout()->addWidget(renderer);
    }
    if (!renderer->getRenderer2d()->lst_layers2d.empty()) {
        renderer->controller->getCamera().centerOnBoundingBox(renderer->getRenderer2d()->lst_layers2d.back()->getBoundingBox());
    }
    renderer->setIs3D(false);
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

    QString filter = is3DMode ? "3D Files (*.obj);;All Files (*.*)" : "2D Files (*.geojson *.shp *.tif *.tiff);;All Files (*.*)";
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
            renderer->getRenderer2d()->session.addToJson(filedata, "vector");
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

            renderer->getRenderer2d()->session.addToJson(filedata, "raster");
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


void MainWindow::on_actionFlux_Data_triggered()
{
    addFluxData dialog(this);

    // Ajouter des URL à la combo box
    dialog.addItemToComboBox_url("WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities");
    //dialog.addItemToComboBox_url("WMTS:https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities");
    dialog.addItemToComboBox_url("WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities");
    dialog.addItemToComboBox_url("WFS:https://data.grandlyon.com/geoserver/wfs?SERVICE=WFS&REQUEST=GetCapabilities&VERSION=1.1.0");
    dialog.addItemToComboBox_url("WMS:https://data.grandlyon.com/geoserver/wms?SERVICE=WMS&REQUEST=GetCapabilities&VERSION=1.3.0");
   // dialog.addItemToComboBox_url("WMTS:https://imagerie.data.grandlyon.com/all/wmts?service=WMTS&request=getcapabilities");

    if (dialog.exec() == QDialog::Accepted) {
        QString layerName_temp = dialog.getLayerName();
        //QString layerName = extractLayerName(layerName_temp);
        QString layerURL = dialog.getLayerURL();

        std::string intername2 = layerName_temp.toStdString();
        std::string  intername1 = layerURL.toStdString();
        const char* layname = intername2.c_str();
        const char* Url = intername1.c_str();

        DataProvider d;
        std::string typef = d.GetTypeStream(Url);


        if (typef == "WFS"){
            API_WFS wfs(Url);
            wfs.loadDataset();
            wfs.ExportToGeoJSON(intername2);
            std::cout << "url" << Url << std::endl;
            std::cout << "layer" << intername2 << std::endl;
            std::cout << wfs.displayMetadata() << std::endl;
            const char* chemin = wfs.getOutput();
            onOpenFile_stream(chemin);
        }

        else if (typef == "WMS"){
            API_WMS wms(Url);
            wms.loadDataset();

            std::cout << "couche intername : " << intername2 << std::endl;
            const char* op = "../cute-gis/data/geotiff/";
            wms.loadTileGridToGeoTiff(layname,1,1,1,1,op);
            const char* chemin2 = wms.getOutput();
            onOpenFile_stream(chemin2);
        } else {
            API_WMTS wmts(Url);
            wmts.loadDataset();

            std::cout << "url" << Url << std::endl;
            std::cout << "layer" << intername2 << std::endl;
            std::cout << wmts.displayMetadata() << std::endl;
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
        if (row >= 0 && row < static_cast<int>(name_layers.size())) {
            std::string layerPath = renderer->getRenderer2d()->lst_layers2d[row]->getName();
            name_layers.erase(name_layers.begin() + row);
            renderer->getRenderer2d()->session.removeFromJson(layerPath.c_str());
            renderer->getRenderer2d()->lst_layers2d.erase(renderer->getRenderer2d()->lst_layers2d.begin() + row);
        }
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
    if (start < 0 || start >= static_cast<int>(renderer->getRenderer2d()->lst_layers2d.size()) ||
        destinationRow < 0 || destinationRow > static_cast<int>(renderer->getRenderer2d()->lst_layers2d.size())) {
        qWarning() << "invalid indices for layer reordering.";
        return;
    }

    auto layer = renderer->getRenderer2d()->lst_layers2d[start];
    renderer->getRenderer2d()->lst_layers2d.erase(renderer->getRenderer2d()->lst_layers2d.begin() + start);

    int adjustedDestination = (destinationRow > start) ? destinationRow - 1 : destinationRow;
    renderer->getRenderer2d()->lst_layers2d.insert(renderer->getRenderer2d()->lst_layers2d.begin() + adjustedDestination, layer);
    auto name = name_layers[start];
    name_layers.erase(name_layers.begin() + start);
    name_layers.insert(name_layers.begin() + adjustedDestination, name);

    renderer->update();
}

void MainWindow::showAttributeTable(const std::shared_ptr<LayerBase>& layer) {
    if (!layer->hasAttributes()) {
        QMessageBox::information(this, "No Attributes", "This layer does not support attribute tables.");
        return;
    }

    const auto& attributes = layer->getAttributes();
    const auto& headers = layer->getAttributeHeaders();

    if (attributes.empty() || headers.empty()) {
        QMessageBox::warning(this, "Empty Attributes", "This layer does not contain any attribute data.");
        return;
    }

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(QString::fromStdString("Attributs : " + layer->getName()));
    dialog->resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QTableWidget* tableWidget = new QTableWidget(dialog);

    tableWidget->setColumnCount(headers.size());
    tableWidget->setRowCount(attributes.size());

    QStringList headerLabels;
    for (const auto& header : headers) {
        headerLabels << QString::fromStdString(header);
    }
    tableWidget->setHorizontalHeaderLabels(headerLabels);

    for (int i = 0; i < attributes.size(); ++i) {
        const auto& row = attributes[i];
        if (row.size() != headers.size()) {
            QMessageBox::critical(this, "Data Error", "Row size does not match header size. Data may be corrupted.");
            return;
        }
        for (int j = 0; j < row.size(); ++j) {
            tableWidget->setItem(i, j, new QTableWidgetItem(QString::fromStdString(row[j])));
        }
    }

    tableWidget->resizeColumnsToContents();
    layout->addWidget(tableWidget);

    dialog->setLayout(layout);

    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, [this, &layer, tableWidget]() {
        QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            renderer->getRenderer2d()->highlightGeometry(layer->getName(), row);

            this->update();
        }
    });

    dialog->exec(); // Afficher la boîte de dialogue
}


void MainWindow::setupCheckboxes() {
    ui->listWidget->clear();

    disconnect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onLayerContextMenuRequested);
    disconnect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::onLayersSuperposed);
    disconnect(ui->listWidget, &QListWidget::itemChanged, nullptr, nullptr);

    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onLayerContextMenuRequested);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::onLayersSuperposed);

    for (const auto& name : name_layers) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name), ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Checked);
    }

    connect(ui->listWidget, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        bool checked = item->checkState() == Qt::Checked;
        QString name = item->text();
        onCheckboxToggled(checked, name.toStdString());
    });

    ui->listWidget->setSpacing(10);
    QFont font = ui->listWidget->font();
    font.setPointSize(15);
    ui->listWidget->setFont(font);
    ui->listWidget->setStyleSheet("QListWidget::item { padding: 5px; font-family: Sans Serif; font-size: 10pt; }");
}
