#include "startwizardpage.h"

#include <QLabel>
#include <QToolButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QVariant>

#include "pinlistimporter.h"

StartWizardPage::StartWizardPage(QWidget *parent)
    : QWizardPage (parent)
{
    _complete = false;
    setAcceptDrops(true);

    setTitle("Choose import format");
    setSubTitle("This wizard will help you import a pin list from various file source");

    QLabel *label = new QLabel("Format:");
    label->setWordWrap(true);

    QButtonGroup *group = new QButtonGroup;
    group->setExclusive(true);
    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(buttonClic(int)));

    QToolButton *buttonCsv = new QToolButton;
    buttonCsv->setIcon(QIcon(":/icons/img/text-csv"));
    buttonCsv->setToolTip("CSV file");
    buttonCsv->setIconSize(QSize(64, 64));
    buttonCsv->setCheckable(true);
    buttonCsv->setEnabled(false);
    group->addButton(buttonCsv, PinListImporter::CSV);

    QToolButton *buttonPdf = new QToolButton;
    buttonPdf->setIcon(QIcon(":/icons/img/application-pdf"));
    buttonPdf->setToolTip("PDF datasheet");
    buttonPdf->setIconSize(QSize(64, 64));
    buttonPdf->setCheckable(true);
    group->addButton(buttonPdf, PinListImporter::PDF);

    /*QToolButton *buttonTable = new QToolButton;
    buttonTable->setIcon(QIcon(":/icons/img/application-x-applix-spreadsheet"));
    buttonTable->setToolTip("XLS excel file");
    buttonTable->setIconSize(QSize(64, 64));
    buttonTable->setCheckable(true);
    //buttonTable->setEnabled(false);
    group->addButton(buttonTable, PinListImporter::Table);*/

    QToolButton *buttonKicad = new QToolButton;
    buttonKicad->setIcon(QIcon(":/icons/img/kicad-lib"));
    buttonKicad->setToolTip("Kicad library");
    buttonKicad->setIconSize(QSize(64, 64));
    buttonKicad->setCheckable(true);
    buttonKicad->setEnabled(false);
    group->addButton(buttonKicad, PinListImporter::Kicad);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(buttonCsv, new QLabel("CSV coma separator (soon)"));
    formLayout->addRow(buttonPdf, new QLabel("PDF datasheet"));
    //formLayout->addRow(buttonTable, new QLabel("Table excel"));
    formLayout->addRow(buttonKicad, new QLabel("Kicad library (soon)"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addLayout(formLayout);
    setLayout(layout);

    //registerField("type", this, "type");
}

int StartWizardPage::nextId() const
{
    return PinListImporter::PageFile;
}

bool StartWizardPage::isComplete() const
{
    return _complete;
}

void StartWizardPage::buttonClic(int type)
{
    static_cast<PinListImporter*>(wizard())->setType((PinListImporter::ImportType)type);
    _complete = true;
    emit completeChanged();
}
