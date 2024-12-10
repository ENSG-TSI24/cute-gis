#ifndef ATTRIBUTETABLEWINDOW_H
#define ATTRIBUTETABLEWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVector>

class AttributeTableWindow : public QDialog {
    Q_OBJECT

public:
    explicit AttributeTableWindow(QWidget *parent = nullptr);
    void populateTable(const QVector<QVector<QString>> &data, const QStringList &headers);


private:
    QTableWidget *tableWidget;
};

#endif // ATTRIBUTETABLEWINDOW_H
