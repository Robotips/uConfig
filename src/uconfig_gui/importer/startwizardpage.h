#ifndef STARTWIZARDPAGE_H
#define STARTWIZARDPAGE_H

#include <QWizardPage>

class StartWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit StartWizardPage(QWidget *parent = 0);

    bool isComplete() const;

    int nextId() const;

private slots:
    void buttonClic(int type);

private:
    bool _complete;
};

#endif // STARTWIZARDPAGE_H
