#include "addFluxData.h"
#include "ui_addFluxData.h"
#include <QMessageBox>
#include <QDebug>
#include "../back/API_WFS.h"
#include "../back/API_WMS.h"
#include "../back/API_WMTS.h"

addFluxData::addFluxData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addFluxData)
{
    ui->setupUi(this);

    // Ajouter un élément par défaut dans la combo box URL
    ui->url->addItem("Sélectionnez une URL...");
    ui->layer->addItem("Aucune couche disponible.");

    // Connecter le signal de changement de sélection de l'URL
    connect(ui->url, &QComboBox::currentIndexChanged, this, &addFluxData::onUrlSelected);
}

addFluxData::~addFluxData()
{
    delete ui;
}

void addFluxData::addItemToComboBox_url(const QString& item)
{
    ui->url->addItem(item);  // Ajouter une URL à la liste
}

QString addFluxData::getLayerName() const
{
    return ui->layer->currentText();  // Récupérer le texte de la couche sélectionnée
}

QString addFluxData::getLayerURL() const
{
    return ui->url->currentText();  // Récupérer le texte de l'URL sélectionnée
}

void addFluxData::onUrlSelected(int index)
{
    QString selectedUrl = ui->url->itemText(index);

    // Ignorer l'élément par défaut ou une sélection invalide
    if (selectedUrl == "Sélectionnez une URL..." || selectedUrl.isEmpty()) {
        ui->layer->clear();
        ui->layer->addItem("Aucune couche disponible.");
        return;
    }

    // Charger les couches pour l'URL sélectionnée
    loadLayersFromURL(selectedUrl);
}

void addFluxData::loadLayersFromURL(const QString& url)
{
    ui->layer->clear();  // Effacer les couches précédentes

    try {
        std::string  intername2 =url.toStdString();
        const char* Url = intername2.c_str();
        DataProvider d;
        std::string typef = d.GetTypeStream(Url);

        if (typef == "WFS"){
            API_WFS wfs(Url);

            wfs.loadDataset();

            if (!wfs.isEmpty()) {
                char** layers = wfs.displayMetadata();  // Récupérer les couches depuis l'URL WFS
                for (int i = 0; layers[i] != nullptr; ++i) {
                    ui->layer->addItem(QString::fromStdString(layers[i]));  // Ajouter chaque couche à la combo box
                }


            } else {
                ui->layer->addItem("Aucune couche disponible.");
                QMessageBox::information(this, "Info", "Aucune couche trouvée pour cette URL.");
            }
        }
        else if (typef == "WMS"){
            API_WMS wms(Url);

            wms.loadDataset();

            if (!wms.isEmpty()) {
                std::cout << "pas rien" << std::endl;
                char** layers = wms.displayMetadata();
                // Récupérer les couches depuis l'URL WFS
                for (int i = 0; layers[i] != nullptr; ++i) {
                    //std::cout << layers[i] << std::endl;

                    ui->layer->addItem(QString::fromStdString(layers[i]));  // Ajouter chaque couche à la combo box
                }


            } else {
                ui->layer->addItem("Aucune couche disponible.");
                QMessageBox::information(this, "Info", "Aucune couche trouvée pour cette URL.");
            }
        }
        else if (typef == "WMTS"){
            API_WMTS wmts(Url);

            wmts.loadDataset();

            if (!wmts.isEmpty()) {
                char** layers = wmts.displayMetadata();  // Récupérer les couches depuis l'URL WFS
                for (int i = 0; layers[i] != nullptr; ++i) {
                    std::cout << layers[i] << std::endl;
                    ui->layer->addItem(QString::fromStdString(layers[i]));  // Ajouter chaque couche à la combo box
                }


            } else {
                ui->layer->addItem("Aucune couche disponible.");
                QMessageBox::information(this, "Info", "Aucune couche trouvée pour cette URL.");
            }
        }
    } catch (const std::exception& e) {
        ui->layer->addItem("Aucune couche disponible.");
        QMessageBox::critical(this, "Erreur", QString("Échec du chargement des couches : %1").arg(e.what()));
    }
}