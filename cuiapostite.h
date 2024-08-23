#ifndef CUIAPOSTITE_H
#define CUIAPOSTITE_H

#include <QMainWindow>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFile>

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

    void on_IDC_RETOUR_clicked();

    void on_IDC_SETEMPLACEMENT_clicked();

    bool fileExists(const QString &filePath);

    void createFile();

    bool emplacementIsSet();

    QString getEmplacement();

private:
    Ui::CUIAPostite *ui;
    int indexMain, indexPara ;
    QSettings *settings;
};
#endif // CUIAPOSTITE_H
