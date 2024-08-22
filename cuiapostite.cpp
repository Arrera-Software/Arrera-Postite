#include "cuiapostite.h"
#include "ui_cuiapostite.h"

CUIAPostite::CUIAPostite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CUIAPostite)
{
    ui->setupUi(this);
}

CUIAPostite::~CUIAPostite()
{
    delete ui;
}

void CUIAPostite::on_IDC_SAVE_clicked()
{
    QString contenuTextEdit;
    QString nomFichier ;
    nomFichier=QFileDialog::getSaveFileName(nullptr,
                                              "Enregistrer le fichier", QDir::homePath(), "Fichier Pense-bete (*.ab)");
    QFile file(nomFichier);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        contenuTextEdit = ui->ZONETEXTE->toPlainText();
        out << contenuTextEdit ;
        QMessageBox::StandardButton reponse = QMessageBox::question(this,
                                                                    "Continuer l'édition ?", "Voulez-vous continuer à éditer ?",
                                                                    QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::No)
        {
            ui->ZONETEXTE->clear();
        }
        file.close();
    }
}


void CUIAPostite::on_IDC_NEW_clicked()
{
    if (!ui->ZONETEXTE->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::StandardButton reponse = QMessageBox::question(this,
                                                                    "Enregistrer votre postite", "Voulez-vous enregistrer votre postite ?",
                                                                    QMessageBox::Yes | QMessageBox::No);
        if (reponse==QMessageBox::Yes)
        {
            QString contenuTextEdit;
            QString nomFichier ;
            nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                      "Enregistrer le fichier", QDir::homePath(), "Fichier Pense-bete (*.ab)");
            QFile file(nomFichier);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                contenuTextEdit = ui->ZONETEXTE->toPlainText();
                out << contenuTextEdit ;
                ui->ZONETEXTE->clear();
                file.close();
            }
        }
        else
        {
            ui->ZONETEXTE->clear();
        }
    }
}


void CUIAPostite::on_IDC_PARA_clicked()
{

}


void CUIAPostite::on_IDC_QUIT_clicked()
{
    close();
}


void CUIAPostite::on_IDC_OPEN_clicked()
{
    QString contenu;
    QString nomFichier ;
    nomFichier = QFileDialog::getOpenFileName(this,
                                              "Choisir un fichier à ouvrir",
                                              QDir::homePath(), "Fichier Pense-bete (*.ab)");
    QFile fichier(nomFichier);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fichier);
        contenu = in.readAll();
        fichier.close();
        ui->ZONETEXTE->setPlainText(contenu);
    }
}

