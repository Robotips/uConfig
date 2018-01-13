#ifndef KSSEDITOR_H
#define KSSEDITOR_H

#include "kicad_global.h"

#include <QPlainTextEdit>

#include "ksssyntax.h"

class KICAD_EXPORT KssEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit KssEditor(QWidget *parent = Q_NULLPTR);

protected:
    KSSSyntax *_syntax;
};

#endif // KSSEDITOR_H
