#include "resultspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>
#include <QPushButton>
#include <QScrollArea>

#include "pinlistimporter.h"

ResultsPage::ResultsPage()
    : QWizardPage(0)
{
    QVBoxLayout *layout = new QVBoxLayout;
    _resultLabel = new QLabel();
    _resultLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    _resultLabel->setWordWrap(true);
    _resultLabel->setScaledContents(true);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(_resultLabel);
    layout->addWidget(scrollArea);
    layout->addSpacerItem(new QSpacerItem(10, 30,
                                          QSizePolicy::Expanding,
                                          QSizePolicy::Expanding));
    setLayout(layout);
}

int ResultsPage::nextId() const
{
    return -1;
}

void ResultsPage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();
    setTitle(QString("Extracted packages in %1").arg(fileName.right(30)));

    QString resText = QString("Packages found:");
    QList<Component *> &components = static_cast<PinListImporter*>(wizard())->components();
    foreach (Component *component, components)
    {
        resText.append(QString("\n - %1 with %2 pins")
                           .arg(component->name())
                           .arg(component->pins().count()));
    }
    _resultLabel->setText(resText);
}

bool ResultsPage::isComplete() const
{
    return true;
}
