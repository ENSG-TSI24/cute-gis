#include "addFluxData.h"
#include "ui_addFluxData.h"

addFluxData::addFluxData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addFluxData) {
    ui->setupUi(this);
}

addFluxData::~addFluxData() {
    delete ui;
}

QString addFluxData::getLayerName() const {
    return ui->lineEdit_Name->text();  // Replace "lineEdit_Name" with the objectName in your UI
}

QString addFluxData::getLayerURL() const {
    return ui->lineEdit_URL->text();  // Replace "lineEdit_URL" with the objectName in your UI
}
