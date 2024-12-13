#ifndef UI_ADDFLUXDATA_H
#define UI_ADDFLUXDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QComboBox>  // Inclure QComboBox

QT_BEGIN_NAMESPACE

class Ui_addFluxData {
public:
    QComboBox *layer;   // Changer pour QComboBox
    QComboBox *url;    // Changer pour QComboBox
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Dialog) {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(400, 300);

        QVBoxLayout *verticalLayout = new QVBoxLayout(Dialog);

        // Création du QComboBox pour 'Layer Name' sans éléments
        layer = new QComboBox(Dialog);
        layer->setObjectName(QString::fromUtf8("layer"));
        verticalLayout->addWidget(layer);

        // Création du QComboBox pour 'URL' sans éléments
        url = new QComboBox(Dialog);
        url->setObjectName(QString::fromUtf8("url"));
        verticalLayout->addWidget(url);

        // Ajout du spacer pour espacer les éléments
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);

        // Configuration des boutons OK et Cancel
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        verticalLayout->addWidget(buttonBox);

        // Connexions des boutons Ok et Cancel
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Dialog, &QDialog::reject);
    }
};

namespace Ui {
    class addFluxData: public Ui_addFluxData {};
}

QT_END_NAMESPACE

#endif // UI_ADDFLUXDATA_H
