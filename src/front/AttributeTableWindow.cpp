#include "AttributeTableWindow.h"

AttributeTableWindow::AttributeTableWindow(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Attribute Table");
    resize(600, 400);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // 默认设置列数
    tableWidget->setHorizontalHeaderLabels({"Field 1", "Field 2", "Field 3"}); // 默认表头

    // 设置布局
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
