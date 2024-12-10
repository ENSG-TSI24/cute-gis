#ifndef ADDFLUXDATA_H
#define ADDFLUXDATA_H

#include <QDialog>

namespace Ui {
class addFluxData;
}

class addFluxData : public QDialog {
    Q_OBJECT

public:
    explicit addFluxData(QWidget *parent = nullptr);
    ~addFluxData();

    QString getLayerName() const;  // Getter for Layer Name
    QString getLayerURL() const;   // Getter for URL

private:
    Ui::addFluxData *ui;
};

#endif // ADDFLUXDATA_H

