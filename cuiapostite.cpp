#include "cuiapostite.h"
#include "ui_cuiapostite.h"

CUIAPostite::CUIAPostite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CUIAPostite)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    indexMain = ui->postite->indexOf(ui->main);
    indexPara = ui->postite->indexOf(ui->para);
    indexColor = ui->postite->indexOf(ui->colorSelect);
    indexView = ui->postite->indexOf(ui->view);
    indexExport = ui->postite->indexOf(ui->pageexport);
    indexInserer = ui->postite->indexOf(ui->insersion);
    indexTableau = ui->postite->indexOf(ui->manageTableau);
    indexAcceuil = ui->postite->indexOf(ui->pageFile);
    connect(ui->IDC_VIEWFILE, &QTreeView::doubleClicked, this, &CUIAPostite::openFileTreeView);
    connect(ui->ZONETEXTE, &MyTextEdit::textChanged, this, &CUIAPostite::onTextChanged);
    nameFile = "";
    if (!fileExists("postite.ini"))
    {
        createFile();
        ui->postite->setCurrentIndex(indexMain);
    }
    else
    {
        QSettings settings("postite.ini", QSettings::IniFormat);
        settings.beginGroup("postite");
        color = settings.value("color").toString();
        setColor(color);
        if (getEmplacement() == "null")
        {
            ui->postite->setCurrentIndex(indexMain);
            ui->IDC_OPEN->setVisible(true);
        }
        else
        {
            setViewFolder();
            ui->postite->setCurrentIndex(indexAcceuil);
            ui->IDC_OPEN->setVisible(false);
        }
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
        nameFile = nomFichier;
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
    ui->IDC_RETOUR->setVisible(true);
    ui->IDC_RETOURACCEUIL->setVisible(false);
}


void CUIAPostite::on_IDC_QUIT_clicked()
{
    if (getEmplacement() == "null")
    {
       close();
    }
    else
    {
        ui->postite->setCurrentIndex(indexAcceuil);
    }
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
    QFile fichier(nomFichier);
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fichier);
        contenu = in.readAll();
        fichier.close();
        ui->ZONETEXTE->setPlainText(contenu);
        nameFile = nomFichier;
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
    setViewFolder();
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
    settings.setValue("lTableau1",5);
    settings.setValue("lTableau2",5);
    settings.setValue("cTableau1",3);
    settings.setValue("cTableau2",10);
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
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 192); color: black;");
        return true;
    }
    else if (color == "white")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 255); color: black;");
        return true;
    } else if (color == "black")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(0, 0, 0)");
        return true;
    } else
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 192); color: white;");
        return false;
    }
}


void CUIAPostite::on_IDC_VIEW_clicked()
{
    ui->VIEWFILEMAKEDOWN->setReadOnly(false);
    ui->VIEWFILEMAKEDOWN->clear();
    ui->VIEWFILEMAKEDOWN->setMarkdown(ui->ZONETEXTE->toPlainText());
    QString styledHtml = applyCssToHtml(ui->VIEWFILEMAKEDOWN->toHtml());
    ui->VIEWFILEMAKEDOWN->clear();
    ui->VIEWFILEMAKEDOWN->setHtml(styledHtml);
    ui->VIEWFILEMAKEDOWN->setReadOnly(true);
    ui->postite->setCurrentIndex(indexView);
}

void CUIAPostite::on_IDC_TITRE1_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("#");
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_TITRE2_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("##");
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_TITRE3_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("###");
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
    ui->VIEWFILEMAKEDOWN->setReadOnly(false);
    ui->VIEWFILEMAKEDOWN->clear();
    ui->VIEWFILEMAKEDOWN->setMarkdown(ui->ZONETEXTE->toPlainText());
    ui->VIEWFILEMAKEDOWN->setReadOnly(true);

    QString htmlContent = ui->VIEWFILEMAKEDOWN->toHtml();

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
    ui->VIEWFILEMAKEDOWN->setReadOnly(false);
    ui->VIEWFILEMAKEDOWN->clear();
    ui->VIEWFILEMAKEDOWN->setMarkdown(ui->ZONETEXTE->toPlainText());
    ui->VIEWFILEMAKEDOWN->setReadOnly(true);
    QString htmlContent = ui->VIEWFILEMAKEDOWN->toHtml();
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

            /* Style pour les tableaux */
            table {
                width: 100%;
                border-collapse: collapse;
                margin: 20px 0;
            }
            th, td {
                padding: 12px;
                border: 5px solid black;
                text-align: left;
            }
            th {
                background-color: #f2f2f2;
                color: #333;
                font-weight: bold;
            }
            tr:nth-child(even) {
                background-color: #f9f9f9;
            }
            tr:hover {
                background-color: #e6f7ff;
            }
        </style>
    )";
    return "<html><head>" + css + "</head><body>" + htmlContent + "</body></html>";
}


void CUIAPostite::on_IDC_ADD_clicked()
{
    ui->postite->setCurrentIndex(indexInserer);
}


void CUIAPostite::on_IDC_RETOURINSERER_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDCHECKBOX_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("- [ ] Texte\n- [ ] Texte");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDLIGNE_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("---");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDCITATION_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("> Texte");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDLISTEPUCE_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("- Texte\n- Texte");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDLISTENUMERO_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("1. Texte\n2. Texte");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDLIENINTERNET_clicked()
{
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText("[Texte du lien](URL_du_lien)");
    ui->ZONETEXTE->setTextCursor(curseur);
    ui->postite->setCurrentIndex(indexMain);
}

// Gestion tableau

void CUIAPostite::on_IDC_PARATABLEAU1_clicked()
{
    ui->postite->setCurrentIndex(indexTableau);
    ui->IDC_PARAMETRAGE1->setVisible(true);
    ui->IDC_PARAMETRAGE2->setVisible(false);
    ui->IDC_ADDTABLEAU->setVisible(false);

}

void CUIAPostite::on_IDC_PARATABLEAU2_clicked()
{
    ui->postite->setCurrentIndex(indexTableau);
    ui->IDC_PARAMETRAGE1->setVisible(false);
    ui->IDC_PARAMETRAGE2->setVisible(true);
    ui->IDC_ADDTABLEAU->setVisible(false);
}


void CUIAPostite::on_IDC_ADDTABLEAU1_clicked()
{
    insertTableau(getColone(1),getLigne(1));
    ui->postite->setCurrentIndex(indexMain);
    ui->IDC_PARAMETRAGE1->setVisible(true);
}


void CUIAPostite::on_IDC_ADDTABLEAU2_clicked()
{
    insertTableau(getColone(2),getLigne(2));
    ui->postite->setCurrentIndex(indexMain);
    ui->IDC_PARAMETRAGE1->setVisible(true);
}

void CUIAPostite::on_TABLEAUAUTRE_clicked()
{
    ui->postite->setCurrentIndex(indexTableau);
    ui->IDC_PARAMETRAGE1->setVisible(false);
    ui->IDC_PARAMETRAGE2->setVisible(false);
    ui->IDC_ADDTABLEAU->setVisible(true);
}

void CUIAPostite::insertTableau(int nbColone, int nbLigne)
{
    QString colone1 = "| Titre ";
    QString colone2 = "|--------";
    QString sortie;
    for (int i = 0; i < nbColone; i++) {
        sortie.append(colone1);
    }
    sortie.append("|\n");
    for (int i = 0; i < nbColone; i++) {
        sortie.append(colone2);
    }
    sortie.append("|\n");

    // Ajout des lignes du tableau
    for (int j = 0; j < nbLigne; j++) {
        for (int i = 0; i < nbColone; i++) {
            sortie.append("| Cellule ");
        }
        sortie.append("|\n");
    }
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(sortie);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_PARAMETRAGE1_clicked()
{
    int nbColone ;
    int nbLigne ;
    QString message ;
    nbColone = ui->IDC_SPINCOLONE->value();
    nbLigne = ui->IDC_SPINLIGNE->value();
    message = "Le tableau numero 1 est "
              "enregistrer avec "+QString::number(nbColone)+
              " colone et "+QString::number(nbLigne)+" lignes";
    ui->IDC_SPINCOLONE->setValue(0);
    ui->IDC_SPINLIGNE->setValue(0);
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("lTableau1",nbLigne);
    settings.setValue("cTableau1",nbColone);
    QMessageBox::information(nullptr,"Information",message);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_PARAMETRAGE2_clicked()
{
    int nbColone ;
    int nbLigne ;
    QString message ;
    nbColone = ui->IDC_SPINCOLONE->value();
    nbLigne = ui->IDC_SPINLIGNE->value();
    message = "Le tableau numero 1 est "
              "enregistrer avec "+QString::number(nbColone)+
              " colone et "+QString::number(nbLigne)+" lignes";
    ui->IDC_SPINCOLONE->setValue(0);
    ui->IDC_SPINLIGNE->setValue(0);
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    settings.setValue("lTableau2",nbLigne);
    settings.setValue("cTableau2",nbColone);
    QMessageBox::information(nullptr,"Information",message);
    ui->postite->setCurrentIndex(indexMain);
}


void CUIAPostite::on_IDC_ADDTABLEAU_clicked()
{
    int nbColone ;
    int nbLigne ;
    nbColone = ui->IDC_SPINCOLONE->value();
    nbLigne = ui->IDC_SPINLIGNE->value();
    ui->IDC_SPINCOLONE->setValue(0);
    ui->IDC_SPINLIGNE->setValue(0);
    ui->postite->setCurrentIndex(indexMain);
    insertTableau(nbColone,nbLigne);
}

void CUIAPostite::on_IDC_CANCELTABLEAU_clicked()
{
    ui->postite->setCurrentIndex(indexMain);
    ui->IDC_SPINCOLONE->setValue(0);
    ui->IDC_SPINLIGNE->setValue(0);
}

int CUIAPostite::getColone(int tab)
{
    int nb ;
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    switch(tab)
    {
        case 1 :
            nb = settings.value("cTableau1").toInt();
            break;
        case 2 :
            nb = settings.value("cTableau2").toInt();
            break;
        default:
            nb = 1 ;
            break ;
    }
    settings.endGroup();
    return nb ;
}
int CUIAPostite::getLigne(int tab)
{
    int nb ;
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    switch(tab)
    {
    case 1 :
        nb = settings.value("lTableau1").toInt();
        break;
    case 2 :
        nb = settings.value("lTableau2").toInt();
        break;
    default:
        nb = 1 ;
        break ;
    }
    settings.endGroup();
    return nb ;
}


void CUIAPostite::setViewFolder()
{
    model->setRootPath(getEmplacement());
    model->setNameFilters(QStringList() << "*.ab");
    model->setNameFilterDisables(false);
    model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->IDC_VIEWFILE->setModel(model);
    ui->IDC_VIEWFILE->setRootIndex(model->index(getEmplacement()));
    ui->IDC_VIEWFILE->hideColumn(1);
    ui->IDC_VIEWFILE->hideColumn(2);
    ui->IDC_VIEWFILE->hideColumn(3);
}

void CUIAPostite::openFileTreeView(const QModelIndex &index)
{
    QString contenu;
    if (model->isDir(index)) {
        return;
    }
    QString filePath = model->filePath(index);
    nameFile = filePath;
    if (filePath.endsWith(".ab"))
    {
        QFile fichier(filePath);
        if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&fichier);
            contenu = in.readAll();
            fichier.close();
            ui->ZONETEXTE->setPlainText(contenu);
        }
        ui->postite->setCurrentIndex(indexMain);
    }
}

void CUIAPostite::on_IDC_PARAMETREACCEUIL_clicked()
{
    ui->postite->setCurrentIndex(indexPara);
    ui->IDC_RETOUR->setVisible(false);
    ui->IDC_RETOURACCEUIL->setVisible(true);
}


void CUIAPostite::on_IDC_RETOURACCEUIL_clicked()
{
    ui->postite->setCurrentIndex(indexAcceuil);
}

void CUIAPostite::on_IDC_QUITACCEUIL_clicked()
{
    close();
}


void CUIAPostite::on_IDC_OPENOTHER_clicked()
{
    on_IDC_OPEN_clicked();
}


void CUIAPostite::on_IDC_ADDFILEACCEUIL_clicked()
{
    bool ok;
    QString nom = "" , nomFichier = getEmplacement()+"/";
    ui->ZONETEXTE->clear();
    ui->postite->setCurrentIndex(indexMain);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sauvergarder", "Voulez-vous sauvegarder ce postite",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        while (nom == "")
        {
            nom= QInputDialog::getText(this, "Nom du fichier",
                                             "Veuillez entrer le nom du postite :",
                                        QLineEdit::Normal, "", &ok);
        }
        nomFichier += nom;
        if (!nomFichier.endsWith(".ab", Qt::CaseInsensitive)) {
            nomFichier += ".ab";
        }

        QFile file(nomFichier);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            QString contenuTextEdit = "";
            out << contenuTextEdit ;
        }
        nameFile = nomFichier;
    }
    else
    {
        QMessageBox::information(nullptr,"Information","Le postite ne sera pas enregistrer");
    }
}

void CUIAPostite::onTextChanged()
{
    if (nameFile.isEmpty() == false)
    {
        QString contenu = ui->ZONETEXTE->toPlainText();

        QFile file(nameFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << contenu ;
        }
    }
}
