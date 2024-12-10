#ifndef UI_ADDFLUXDATA_H
#define UI_ADDFLUXDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_addFluxData {
public:
    QLineEdit *lineEdit_Name;
    QLineEdit *lineEdit_URL;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Dialog) {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(400, 300);

        QVBoxLayout *verticalLayout = new QVBoxLayout(Dialog);

        QLabel *label_Name = new QLabel(Dialog);
        label_Name->setText("Layer Name:");
        verticalLayout->addWidget(label_Name);

        lineEdit_Name = new QLineEdit(Dialog);
        lineEdit_Name->setObjectName(QString::fromUtf8("lineEdit_Name"));
        verticalLayout->addWidget(lineEdit_Name);

        QLabel *label_URL = new QLabel(Dialog);
        label_URL->setText("URL:");
        verticalLayout->addWidget(label_URL);

        lineEdit_URL = new QLineEdit(Dialog);
        lineEdit_URL->setObjectName(QString::fromUtf8("lineEdit_URL"));
        verticalLayout->addWidget(lineEdit_URL);

        // Add Spacer
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        verticalLayout->addWidget(buttonBox);

        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Dialog, &QDialog::reject);
    }
};

namespace Ui {
    class addFluxData: public Ui_addFluxData {};
}

QT_END_NAMESPACE

#endif // UI_ADDFLUXDATA_H


