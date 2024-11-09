#include "cuiapostite.h"
#include <cmark.h>
#include "ui_cuiapostite.h"

CUIAPostite::CUIAPostite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CUIAPostite)
{
    ui->setupUi(this);
    indexMain = ui->postite->indexOf(ui->main);
    indexPara = ui->postite->indexOf(ui->para);
    indexColor = ui->postite->indexOf(ui->colorSelect);
    indexView = ui->postite->indexOf(ui->view);
    indexExport = ui->postite->indexOf(ui->pageexport);
    ui->postite->setCurrentIndex(indexMain);
    if (!fileExists("postite.ini"))
    {
        createFile();
    }
    else
    {
        QSettings settings("postite.ini", QSettings::IniFormat);
        settings.beginGroup("postite");
        color = settings.value("color").toString();
        setColor(color);
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

    // Vérifie si l'extension est manquante et l'ajoute si nécessaire
    if (!nomFichier.endsWith(".ab", Qt::CaseInsensitive)) {
        nomFichier += ".ab";
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
    settings.setValue("color", "yellow");
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

void CUIAPostite::on_IDC_SETCOLOR_clicked()
{
    ui->postite->setCurrentIndex(indexColor);
}


void CUIAPostite::on_IDC_RETOURCOLOR_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_YELLOW_clicked()
{
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("color", "yellow");
    setColor("yellow");
    settings.endGroup();
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_WHITE_clicked()
{
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("color", "white");
    setColor("white");
    settings.endGroup();
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_BLACK_clicked()
{
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("color", "black");
    setColor("black");
    settings.endGroup();
    ui->postite->setCurrentIndex(indexMain);
}

bool CUIAPostite::setColor(QString color)
{
    if (color == "yellow")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 192)");
        return true;
    }
    else if (color == "white")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 255)");
        return true;
    } else if (color == "black")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(0, 0, 0)");
        return true;
    } else
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 192)");
        return false;
    }
}


void CUIAPostite::on_IDC_VIEW_clicked()
{
    QString content = converseMD();
    ui->VIEWFILEMAKEDOWN->setReadOnly(false);
    ui->VIEWFILEMAKEDOWN->clear();
    ui->VIEWFILEMAKEDOWN->setHtml(content);
    ui->VIEWFILEMAKEDOWN->setReadOnly(true);
    ui->postite->setCurrentIndex(indexView);
}


void CUIAPostite::on_IDC_TITRE1_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("#");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_TITRE2_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("##");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_TITRE3_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("###");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_GRAS_clicked()
{
    QString texteAInserer = "****";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ITALIQUE_clicked()
{
    QString texteAInserer = "__";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_BARRE_clicked()
{
    QString texteAInserer = "~~~~";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}

void CUIAPostite::on_IDC_RETOUREDITEUR_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
}

QString CUIAPostite::converseMD()
{
    QString markdownData = ui->ZONETEXTE->toPlainText();

    // Convertit markdownData en UTF-8 pour l'utiliser avec cmark
    QByteArray utf8Data = markdownData.toUtf8();

    // Utilise cmark pour convertir le texte Markdown en HTML
    char *htmlData = cmark_markdown_to_html(utf8Data.constData(), utf8Data.size(), CMARK_OPT_DEFAULT);

    // Convertit le résultat en QString
    QString html = QString::fromUtf8(htmlData);

    // Libère la mémoire allouée par cmark
    free(htmlData);

    return html;
}


void CUIAPostite::on_IDC_EXPORT_clicked()
{
    ui->postite->setCurrentIndex(indexExport);
}


void CUIAPostite::on_IDC_RETOUREXPORT_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_PRINTPDF_clicked()
{
    QString htmlContent = converseMD();

    // 2. Appliquer un style CSS pour l'impression
    QString styledHtml = applyCssToHtml(htmlContent);

    // 3. Créer une instance de QPrinter en mode PDF
    QPrinter printer(QPrinter::HighResolution);

    // 4. Définir le fichier de sortie (le PDF)
    QString outputPath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if (outputPath.isEmpty()) {
        return;  // L'utilisateur a annulé
    }

    if (!outputPath.endsWith(".pdf", Qt::CaseInsensitive)) {
        outputPath += ".pdf";
    }

    printer.setOutputFileName(outputPath);  // Définir le fichier PDF de sortie

    // 5. Imprimer le contenu HTML dans le fichier PDF
    QTextBrowser *browser = new QTextBrowser();
    browser->setHtml(styledHtml);  // Charger le HTML stylisé
    browser->print(&printer);  // Imprimer dans le fichier PDF

    // 6. Libérer les ressources
    delete browser;
}


void CUIAPostite::on_IDC_PRINDMD_clicked()
{
    QString contenuTextEdit;
    QString nomFichier;

    nomFichier = QFileDialog::getSaveFileName(nullptr,
                                              "Enregistrer le fichier",
                                              QDir::homePath(),
                                              "Markdown (*.md)");

    // Vérifie si l'extension est manquante et l'ajoute si nécessaire
    if (!nomFichier.endsWith(".md", Qt::CaseInsensitive)) {
        nomFichier += ".md";
    }

    QFile file(nomFichier);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        contenuTextEdit = ui->ZONETEXTE->toPlainText();
        out << contenuTextEdit;
        file.close();
    }
}


void CUIAPostite::on_IDC_PRINT_clicked()
{
    QString htmlContent = converseMD();
    QString styledHtml = applyCssToHtml(htmlContent);
    QTextBrowser *browser = new QTextBrowser();
    browser->setHtml(styledHtml);
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Rejected) {
        return;
    }
    browser->print(&printer);
    delete browser;
}


QString CUIAPostite::applyCssToHtml(const QString &htmlContent) {
    QString css = R"(
        <style>
            body { font-family: Arial, sans-serif; margin: 20px; }
            h1, h2, h3 { color: #333; }
            p { color: #555; line-height: 1.6; }
            code { font-family: 'Courier New', monospace; background-color: #f4f4f4; padding: 2px 4px; }
        </style>
    )";
    return "<html><head>" + css + "</head><body>" + htmlContent + "</body></html>";
}


void CUIAPostite::on_IDC_ADD_clicked()
{

}

