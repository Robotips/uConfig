#ifndef STARTWIZARDPAGE_H
#define STARTWIZARDPAGE_H

#include <QWizardPage>

class StartWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit StartWizardPage(QWidget *parent = 0);

    bool isComplete() const;

    Q_PROPERTY(int type READ type WRITE setType)

    int type();
    void setType(int type);

    int nextId() const;

private slots:
    void buttonClic(int type);

private:
    bool _complete;
    int _type;
};

#endif // STARTWIZARDPAGE_H
