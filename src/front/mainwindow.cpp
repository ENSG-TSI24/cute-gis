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
#include "addFluxData.h"
#include "geotiffloader.h"
#include "renderer2d.h"
#include "renderer3d.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , renderer(new Renderer(this))
    , refreshTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->actionfiles, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->button_3d, &QPushButton::clicked, this, &MainWindow::onToggle3DMode);

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
        renderer->renderer2d->reset2D();
        name_layers.clear();
        setupCheckboxes();
        QMessageBox::information(this, "mode Changed", "3D mode activated");
    } else {
        renderer->renderer3d->reset3D();
        QMessageBox::information(this, "Mode Changed", "2D mode activated");
    }

    ui->button_3d->setText(!isCurrently3D ? "Switch to 2D Mode" : "Switch to 3D Mode");
}

void MainWindow::onOpenFile()
{
    bool is3DMode = renderer->getIs3D();

    QString filter = is3DMode ? "OBJ Files (*.obj);;All Files (*.*)" : "GeoJSON Files (*.geojson);;All Files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(this, "Open File ...", "../cute-gis/data", filter);

    if (filePath.isEmpty()) {
        qWarning() << "No file selected!";
        return;
    }

    qDebug() << "Selected File:" << filePath;
    std::string filestr = filePath.toStdString();
    const char* filedata = filestr.c_str();
    renderer->renderer3d->reset3D();

    try {
        if (filePath.endsWith(".geojson", Qt::CaseInsensitive)) {
            renderer->renderer3d->reset3D();
            //add layer2d
            std::cout<<"############### ADD LAYER ################"<<std::endl;

            
            VectorData geo(filedata);
            Layer2d newLayer(geo, filePath.toStdString());
            renderer->renderer2d->lst_layers2d.push_back(newLayer);
//            renderer->renderer2d->lst_layers2d.push_back(geo);

            // add name layers
            QFileInfo fileInfo(filePath);
            std::string name = fileInfo.baseName().toStdString();
            renderer->renderer2d->lst_layers2d.back().name = name;
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;
            renderer->controller->getCamera().centerOnBoundingBox(renderer->renderer2d->lst_layers2d.back().boundingBox);
            renderer->setIs3D(false);

        } else if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
            renderer->renderer2d->reset2D();
            nb_layers=0;
            ObjectLoader* objectLoader = new ObjectLoader(filedata, this);
            renderer->renderer3d->setObjectLoader(objectLoader);
            renderer->setIs3D(true);
        } else if (filePath.endsWith(".tif", Qt::CaseInsensitive) || filePath.endsWith(".tiff", Qt::CaseInsensitive)) {
            renderer->renderer2d->reset2D();
            GeoTiffLoader loader;
            loader.loadGeoTIFF(filePath);
            QImage* image = loader.image;

            renderer->renderer2d->lst_layersraster.push_back(LayerRaster(image));

            // add name layers
            std::string name = "Couche " + std::to_string(nb_layers);
            name_layers.push_back(name);
            setupCheckboxes();
            ++nb_layers;
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

    for (auto& layer : renderer->renderer2d->lst_layers2d) {
        if (layer.name == name) {
            layer.isVisible = checked;
        }
    }

}

void MainWindow::onLayerContextMenuRequested(const QPoint& pos) {
    QListWidget* listWidget = qobject_cast<QListWidget*>(sender());
    if (!listWidget) return;

    QListWidgetItem* item = listWidget->itemAt(pos);
    if (!item) return;

    QMenu contextMenu(this);

    QAction* zoomLayer = contextMenu.addAction("Zoom");
    QAction* renameAction = contextMenu.addAction("Rename");
    QAction* deleteAction = contextMenu.addAction("Delete");

    QAction* attributeAction = contextMenu.addAction("Open Attribute Table", this, [this, item]() { openAttributeTable(item); });


    QAction* selectedAction = contextMenu.exec(listWidget->mapToGlobal(pos));

    int row = listWidget->row(item);
        if (row < 0 || row >= static_cast<int>(renderer->renderer2d->lst_layers2d.size())) return;

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
        renderer->renderer2d->lst_layers2d.erase(renderer->renderer2d->lst_layers2d.begin() + row);
    } else if  (selectedAction == zoomLayer) {
        const Layer2d& layer = renderer->renderer2d->lst_layers2d[row];
        renderer->controller->getCamera().centerOnBoundingBox(layer.boundingBox);

        // falcultative
        QMessageBox::information(this, "Zoom", "Zoomed to layer: " + QString::fromStdString(layer.name));
    }

    // Open Attribute Table
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


    // Create QListWidget
    layerListWidget = new QListWidget(ui->layer_manager);
    layerListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    layerListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(layerListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onLayerContextMenuRequested);

    for (const auto& name : name_layers) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name), layerListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Checked);
    }


    // Set layer_manager main layout
    auto* layout = new QVBoxLayout(ui->layer_manager);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(listWidget);
    layout->addStretch();

    layout->addWidget(layerListWidget);

    ui->layer_manager->setLayout(layout);

}



QVector<QVector<QString>> parseGeoJson(const QString &filePath, QStringList &headers) {
    QVector<QVector<QString>> tableData;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return tableData;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Invalid GeoJSON file.";
        return tableData;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonArray features = rootObj["features"].toArray();

    // Parse features
    for (const QJsonValue &feature : features) {
        QJsonObject featureObj = feature.toObject();
        QJsonObject properties = featureObj["properties"].toObject();

        // Collect headers
        if (headers.isEmpty()) {
            headers = properties.keys();
        }

        // Collect row data
        QVector<QString> row;
        for (const QString &key : headers) {
            row.append(properties[key].toString());
        }
        tableData.append(row);
    }

    return tableData;
}



void MainWindow::openAttributeTable(QListWidgetItem* item) {
    if (!item || !layerListWidget) return;

    int row = layerListWidget->row(item);
    if (row < 0 || row >= static_cast<int>(renderer->renderer2d->lst_layers2d.size())) return;

    // get the layer
    Layer2d& layer = renderer->renderer2d->lst_layers2d[row];

    if (layer.properties.empty()) {
        QMessageBox::warning(this, "Error", "No properties available for this layer.");
        return;
    }

    // prepare table data
    QStringList headers;
    QVector<QVector<QString>> tableData;

    for (const auto& property : layer.properties) {
        if (headers.isEmpty()) {
            headers = property.keys();
        }
        QVector<QString> rowData;
        for (const QString& key : headers) {
            rowData.append(property[key].toString());
        }
        tableData.append(rowData);
    }

    // create AttributeTableWindow
    AttributeTableWindow* attrWindow = new AttributeTableWindow(this);
    attrWindow->populateTable(tableData, headers);
    attrWindow->exec();
}
