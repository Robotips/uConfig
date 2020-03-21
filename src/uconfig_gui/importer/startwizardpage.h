#ifndef STARTWIZARDPAGE_H
#define STARTWIZARDPAGE_H

#include <QWizardPage>

class StartWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit StartWizardPage(QWidget *parent = nullptr);

    bool isComplete() const override;

    int nextId() const override;

private slots:
    void buttonClic(int type);

private:
    bool _complete;
};

#endif // STARTWIZARDPAGE_H
