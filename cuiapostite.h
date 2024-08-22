#ifndef CUIAPOSTITE_H
#define CUIAPOSTITE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CUIAPostite;
}
QT_END_NAMESPACE

class CUIAPostite : public QMainWindow
{
    Q_OBJECT

public:
    CUIAPostite(QWidget *parent = nullptr);
    ~CUIAPostite();

private:
    Ui::CUIAPostite *ui;
};
#endif // CUIAPOSTITE_H
