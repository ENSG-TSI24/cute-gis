#ifndef ADDFLUXDATA_H
#define ADDFLUXDATA_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class addFluxData;
}

class addFluxData : public QDialog
{
    Q_OBJECT

public:
    explicit addFluxData(QWidget *parent = nullptr);
    ~addFluxData();

    void addItemToComboBox_url(const QString& item);
    QString getLayerName() const;
    QString getLayerURL() const;

private slots:
    void onUrlSelected(int index);

private:
    void loadLayersFromURL(const QString& url);

    Ui::addFluxData *ui;
};

#endif // ADDFLUXDATA_H