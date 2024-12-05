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
    // 清除所有布局项
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                qDebug() << "Deleting widget:" << item->widget();
                item->widget()->deleteLater();  // 删除 widget
            }
            delete item;  // 删除 layoutItem
        }
    }
}

void MainWindow::onCheckboxToggled(bool checked, std::string name) {

    std::cout<<"-------------- checkbox clicked --------------"<<std::endl;
    std::cout<<name<<std::endl;

    for (auto& layer : renderer->lst_layers2d) {
        if (layer.name == name) {
            layer.isVisible = checked;
        }
    }

}



void MainWindow::setupCheckboxes() {
    // 清理旧的布局
    if (ui->layer_manager->layout()) {
        clearLayout(ui->layer_manager->layout());
        delete ui->layer_manager->layout();
        qDebug() << "Cleared the existing layout";

    }

    // 创建新的布局
    QVBoxLayout *layout = new QVBoxLayout(ui->layer_manager);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setContentsMargins(10, 10, 10, 10);

    // 创建新的复选框和删除按钮
    for (size_t i = 0; i < name_layers.size(); ++i) {
        const auto& name = name_layers[i];

        QHBoxLayout *hLayout = new QHBoxLayout();
        QCheckBox *checkbox = new QCheckBox(QString::fromStdString(name), ui->layer_manager);
        qDebug() << "Creating checkbox for layer:" << QString::fromStdString(name);
        
        checkbox->setStyleSheet("QCheckBox::indicator { width: 16px; height: 16px; }"
                                "QCheckBox { padding-left: 5px; "
                                "            font-family: Sans Serif;"
                                "            font-size:    12pt; "
                                "            font-weight: normal;}"
                                );

        checkbox->setChecked(true);

        // 连接复选框的信号
        connect(checkbox, &QCheckBox::toggled, [this, name](bool checked) {
            onCheckboxToggled(checked, name);
        });

        checkbox->setFixedHeight(20);

        // 删除按钮
        QPushButton *deleteButton = new QPushButton("Delete", ui->layer_manager);
        deleteButton->setStyleSheet("font-size: 10pt;");
        deleteButton->setFixedSize(60, 20);

        // 连接删除按钮的信号
        connect(deleteButton, &QPushButton::clicked, [this, i]() {
            onDeleteLayer(i);
        });

        // 添加到布局
        hLayout->addWidget(checkbox);
        hLayout->addWidget(deleteButton);
        layout->addLayout(hLayout);
        layout->addSpacing(10);
    }

    // 添加拉伸，保证布局填满空间
    layout->addStretch();

    // 设置新的布局
    ui->layer_manager->setLayout(layout);

    ui->layer_manager->update();
    qDebug() << "Checkboxes and layout updated";
}

void MainWindow::onDeleteLayer(size_t index) {
    if (index >= renderer->lst_layers2d.size()) {
        qWarning() << "Invalid layer index!";
        return;
    }
    qDebug() << "Deleting layer at index:" << index;

    // 删除图层
    renderer->lst_layers2d.erase(renderer->lst_layers2d.begin() + index);
    name_layers.erase(name_layers.begin() + index);
    qDebug() << "Remaining layers:" << name_layers.size();

    clearLayout(ui->layer_manager->layout());


    // 删除与该图层相关的复选框和删除按钮
    setupCheckboxes();

    // 重新渲染
    ui->layer_manager->update();
    qDebug() << "UI updated after layer deletion";
}


