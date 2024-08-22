#ifndef CUIAPOSTITE_H
#define CUIAPOSTITE_H

#include <QMainWindow>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

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

private slots:
    void on_IDC_SAVE_clicked();

    void on_IDC_NEW_clicked();

    void on_IDC_PARA_clicked();

    void on_IDC_QUIT_clicked();

    void on_IDC_OPEN_clicked();

private:
    Ui::CUIAPostite *ui;
};
#endif // CUIAPOSTITE_H
