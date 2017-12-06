#ifndef KSSEDITOR_H
#define KSSEDITOR_H

#include "kicad_global.h"

#include <QPlainTextEdit>

class KICAD_EXPORT KssEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit KssEditor(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // KSSEDITOR_H
