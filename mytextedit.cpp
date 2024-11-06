#include "mytextedit.h"
#include <QMimeData>

MyTextEdit::MyTextEdit(QWidget *parent)
    : QTextEdit(parent) {}

void MyTextEdit::insertFromMimeData(const QMimeData *source) {
    if (source->hasText()) {
        insertPlainText(source->text());
    } else {
        QTextEdit::insertFromMimeData(source);
    }
}
