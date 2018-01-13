#include "ksseditor.h"

KssEditor::KssEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    _syntax = new KSSSyntax(this->document());
    QFont font("Consolas");
    font.setPixelSize(12);
    font.setStyleHint(QFont::Monospace);
    setFont(font);
}
