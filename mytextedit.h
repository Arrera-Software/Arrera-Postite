#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
#include <QMimeData>

class MyTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit MyTextEdit(QWidget *parent = nullptr);

protected:
    void insertFromMimeData(const QMimeData *source) override;
};

#endif // MYTEXTEDIT_H
