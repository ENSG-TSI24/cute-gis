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
    return ui->lineEdit_Name->text();
}

QString addFluxData::getLayerURL() const {
    return ui->lineEdit_URL->text();
}
