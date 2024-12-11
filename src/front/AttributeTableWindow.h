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

signals:
    void elementSelected(int row, int column); 
    void rowSelected(int row);

private:
    QTableWidget *tableWidget;

private slots:
    void handleItemSelection(); // 新增槽函数，处理用户选择
};

#endif // ATTRIBUTETABLEWINDOW_H
