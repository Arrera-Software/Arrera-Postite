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
#include <QInputDialog>
#include <QShortcut>
#include <QKeySequence>

// Partie debug
#include <iostream>
using namespace std;

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

    bool setColor();

    void on_IDC_TITRE1_clicked();

    void on_IDC_TITRE2_clicked();

    void on_IDC_TITRE3_clicked();

    void on_IDC_GRAS_clicked();

    void on_IDC_ITALIQUE_clicked();

    void on_IDC_BARRE_clicked();

    void on_IDC_PRINTPDF_clicked();

    void on_IDC_PRINDMD_clicked();

    void on_IDC_PRINT_clicked();

    void on_IDC_ADDCHECKBOX_clicked();

    void on_IDC_ADDLIGNE_clicked();

    void on_IDC_ADDCITATION_clicked();

    void on_IDC_ADDLISTEPUCE_clicked();

    void on_IDC_ADDLISTENUMERO_clicked();

    void on_IDC_ADDLIENINTERNET_clicked();

    void on_IDC_ADDTABLEAU_clicked();

    void on_IDC_CANCELTABLEAU_clicked();

    void openFileTreeView(const QModelIndex &index);

    void on_IDC_PARAMETREACCEUIL_clicked();

    void on_IDC_QUITACCEUIL_clicked();

    void on_IDC_OPENOTHER_clicked();

    void on_IDC_ADDFILEACCEUIL_clicked();

    void onTextChanged();

    void on_IDC_3X3_clicked();

    void on_IDC_6X6_clicked();

    void on_IDC_10X10_clicked();

    void on_IDC_OTHER_clicked();

    void on_IDC_SAVEHTML_clicked();

    void on_IDC_INSERECODE_clicked();

    void on_IDC_BTNEDITVIEW_clicked();

    void on_IDC_BTNEDIT_clicked();

    void on_IDC_BTNVIEW_clicked();

private:
    Ui::CUIAPostite *ui;
    QFileSystemModel *model;
    int indexMain, indexPara,indexTableau,indexAcceuil ;
    int indexOngletFichier,indexOngletTexte,indexOngletTableau,indexOngletInserer,indexOngletExport;
    QString color,nameFile;
    QSettings *settings;
    void  insertTableau(int nbColone,int nbLigne);
    QString getEmplacement();
    int getColone(int tab);
    int getLigne(int tab);
    void setViewFolder();
    // Partie raccourci clavier
    QShortcut *shortcutOpen;
    QShortcut *shortcutSave;
    QShortcut *shortcutNew;

};
#endif // CUIAPOSTITE_H
