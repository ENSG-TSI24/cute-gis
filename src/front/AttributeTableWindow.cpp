#include "AttributeTableWindow.h"

AttributeTableWindow::AttributeTableWindow(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Attribute Table");
    resize(300, 300);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"Field 1", "Field 2", "Field 3"});

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}



void AttributeTableWindow::populateTable(const QVector<QVector<QString>> &data, const QStringList &headers) {
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    tableWidget->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[i].size(); ++j) {
            tableWidget->setItem(i, j, new QTableWidgetItem(data[i][j]));
        }
    }
}
