#include "cuiapostite.h"
#include "ui_cuiapostite.h"

CUIAPostite::CUIAPostite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CUIAPostite)
{
    ui->setupUi(this);
    indexMain = ui->postite->indexOf(ui->main);
    indexPara = ui->postite->indexOf(ui->para);
    ui->postite->setCurrentIndex(indexMain);
    if (!fileExists("postite.ini"))
    {
        createFile();
    }
}

CUIAPostite::~CUIAPostite()
{
    delete ui;
}

void CUIAPostite::on_IDC_SAVE_clicked()
{
    QString contenuTextEdit;
    QString nomFichier ;
    if (!emplacementIsSet())
    {
        nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                  "Enregistrer le fichier", QDir::homePath(), "Fichier Pense-bete (*.ab)");
    }
    else
    {
        nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                  "Enregistrer le fichier", getEmplacement(), "Fichier Pense-bete (*.ab)");
    }

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
            if (!emplacementIsSet())
            {
                nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                      "Enregistrer le fichier", QDir::homePath(), "Fichier Pense-bete (*.ab)");
            }
            else
            {
                nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                          "Enregistrer le fichier", getEmplacement(), "Fichier Pense-bete (*.ab)");
            }
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
    ui->postite->setCurrentIndex(indexPara);
}


void CUIAPostite::on_IDC_QUIT_clicked()
{
    close();
}


void CUIAPostite::on_IDC_OPEN_clicked()
{
    QString contenu;
    QString nomFichier ;
    if (!emplacementIsSet())
    {
        nomFichier = QFileDialog::getOpenFileName(this,
                                                  "Choisir un fichier à ouvrir",
                                                  QDir::homePath(), "Fichier Pense-bete (*.ab)");
    }
    else
    {
        nomFichier = QFileDialog::getOpenFileName(this,
                                                  "Choisir un fichier à ouvrir",
                                                  getEmplacement(), "Fichier Pense-bete (*.ab)");
    }
    QFile fichier(nomFichier);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fichier);
        contenu = in.readAll();
        fichier.close();
        ui->ZONETEXTE->setPlainText(contenu);
    }
}


void CUIAPostite::on_IDC_RETOUR_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_SETEMPLACEMENT_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this,
                                      "Choisir un dossier",
                                      QDir::homePath(),
                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (folder.isEmpty())
    {
        QMessageBox::warning(this,"Arrera Postite","Aucun dossier selectionner");
    }
    else
    {
        QSettings settings("postite.ini", QSettings::IniFormat);
        settings.beginGroup("postite");
        settings.setValue("emplacement", folder);
        settings.endGroup();
        QMessageBox::information(this,"Arrera Postite","Dossier enregistrer");
    }
}

bool CUIAPostite::fileExists(const QString &filePath)
{
    QFile file(filePath);
    return file.exists();
}

void CUIAPostite::createFile()
{
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("emplacement", "null");
    settings.endGroup();
}

bool CUIAPostite::emplacementIsSet()
{
    return getEmplacement().isEmpty();
}

QString CUIAPostite::getEmplacement()
{
    QString var ;
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    var = settings.value("emplacement").toString();
    settings.endGroup();
    return var;
}
