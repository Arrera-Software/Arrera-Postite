#ifndef CUIAPOSTITE_H
#define CUIAPOSTITE_H

#include <QMainWindow>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QProcess>
#include <QPrinter>
#include <QTextBrowser>
#include <QPrintDialog>
#include <QTextDocument>
#include <QFileSystemModel>

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

    void on_IDC_SETCOLOR_clicked();

    void on_IDC_RETOURCOLOR_clicked();

    void on_IDC_YELLOW_clicked();

    void on_IDC_WHITE_clicked();

    void on_IDC_BLACK_clicked();

    bool setColor(QString color);

    void on_IDC_VIEW_clicked();

    void on_IDC_TITRE1_clicked();

    void on_IDC_TITRE2_clicked();

    void on_IDC_TITRE3_clicked();

    void on_IDC_GRAS_clicked();

    void on_IDC_ITALIQUE_clicked();

    void on_IDC_BARRE_clicked();

    void on_IDC_RETOUREDITEUR_clicked();

    void on_IDC_EXPORT_clicked();

    void on_IDC_RETOUREXPORT_clicked();

    void on_IDC_PRINTPDF_clicked();

    void on_IDC_PRINDMD_clicked();

    void on_IDC_PRINT_clicked();

    void on_IDC_ADD_clicked();

    void on_IDC_RETOURINSERER_clicked();

    void on_IDC_ADDCHECKBOX_clicked();

    void on_IDC_ADDLIGNE_clicked();

    void on_IDC_ADDCITATION_clicked();

    void on_IDC_ADDLISTEPUCE_clicked();

    void on_IDC_ADDLISTENUMERO_clicked();

    void on_IDC_ADDLIENINTERNET_clicked();

    void on_IDC_PARATABLEAU1_clicked();

    void on_IDC_PARATABLEAU2_clicked();

    void on_IDC_ADDTABLEAU1_clicked();

    void on_IDC_ADDTABLEAU2_clicked();

    void on_TABLEAUAUTRE_clicked();

    void on_IDC_PARAMETRAGE1_clicked();

    void on_IDC_PARAMETRAGE2_clicked();

    void on_IDC_ADDTABLEAU_clicked();

    void on_IDC_CANCELTABLEAU_clicked();

private:
    Ui::CUIAPostite *ui;
    QFileSystemModel *model;
    int indexMain, indexPara ,indexColor,indexView,indexExport,indexInserer,indexTableau,indexAcceuil ;
    QString color;
    QSettings *settings;
    QString applyCssToHtml(const QString &htmlContent);
    void  insertTableau(int nbColone,int nbLigne);
    QString getEmplacement();
    int getColone(int tab);
    int getLigne(int tab);
    void setViewFolder();
private slots:
    void openFileTreeView(const QModelIndex &index);
};
#endif // CUIAPOSTITE_H
