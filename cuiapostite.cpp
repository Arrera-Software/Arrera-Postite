#include "cuiapostite.h"
#include "ui_cuiapostite.h"

CUIAPostite::CUIAPostite(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CUIAPostite),
    tigerDemon("https://arrera-software.fr/depots.json","arrera-postite",this),
    winUpdate(this),
    socket(this,"arrera-postite")
{
    ui->setupUi(this);
    // Init des var
    model = new QFileSystemModel(this);
    nameFile = "";
    typeFile = "ab";
    pageApropos = 0;

    // Index Page
    indexMain = ui->postite->indexOf(ui->main);
    indexPara = ui->postite->indexOf(ui->para);
    indexTableau = ui->postite->indexOf(ui->manageTableau);
    indexFile = ui->postite->indexOf(ui->pageFile);
    indexAcceuil = ui->postite->indexOf(ui->acceuil);
    indexApropos = ui->postite->indexOf(ui->apropos);

    // Index onglets
    indexOngletFichier = ui->tabonsere->indexOf(ui->Fichier);
    indexOngletTexte = ui->tabonsere->indexOf(ui->texte);
    indexOngletTableau = ui->tabonsere->indexOf(ui->tableau);
    indexOngletInserer = ui->tabonsere->indexOf(ui->inserer);
    indexOngletExport = ui->tabonsere->indexOf(ui->exportPage);

    // Mise en place des raccourci clavier
    shortcutNew = new QShortcut(QKeySequence(tr("Ctrl+N")), this);
    shortcutSave = new QShortcut(QKeySequence(tr("Ctrl+S")), this);
    shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O")), this);

    // Connect
    connect(ui->IDC_VIEWFILE, &QTreeView::doubleClicked, this, &CUIAPostite::openFileTreeView);
    connect(ui->ZONETEXTE, &MyTextEdit::textChanged, this, &CUIAPostite::onTextChanged);
    connect(shortcutNew, &QShortcut::activated, this,&CUIAPostite::on_IDC_NEW_clicked);
    connect(shortcutSave, &QShortcut::activated, this,&CUIAPostite::on_IDC_SAVE_clicked);
    connect(shortcutSave, &QShortcut::activated, this,&CUIAPostite::on_IDC_OPEN_clicked);

    // Partie parametre
    if (!fileExists("postite.ini"))
    {
        createFile();
        ui->postite->setCurrentIndex(indexAcceuil);
    }
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    color = settings.value("color").toString();
    setColor();
    if (getEmplacement() == "null")
    {
        ui->postite->setCurrentIndex(indexAcceuil);
    }
    else{
        setViewFolder();
        ui->postite->setCurrentIndex(indexFile);
    }

    // Set de l'ongets par default
    ui->tabonsere->setCurrentIndex(indexOngletFichier);

    // Mise en place de l'affichage du numero de version
    ui->IDC_VERSION->setText(tigerDemon.getVersionSoft());

    // Initilisation de socket
    if (socket.connectToServeur("ws://127.0.0.1:12345")){

        connect(&socket, &CArreraClient::connectionEstablished, [this]() {
            if (socket.sendMessage("Je suis le client"))
                cout << "Message envoyé" << endl;
            else
                cout << "Erreur: Impossible d'envoyer le message" << endl;
        });

        connect(&socket, &CArreraClient::messageReceived, [this](const QString& message) {
            traitementSocket(message);
        });
    }
}

void CUIAPostite::show(){
    QMainWindow::show();

    if (tigerDemon.checkUpdate()){
        winUpdate.show();
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
    if (typeFile == "ab"){
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
    }else{
        nomFichier=QFileDialog::getSaveFileName(nullptr,
                                                  "Enregistrer le fichier", QDir::homePath(), "Fichiers Markdown (*.md)");

        // Vérifie si l'extension est manquante et l'ajoute si nécessaire
        if (!nomFichier.endsWith(".md", Qt::CaseInsensitive)) {
            nomFichier += ".md";
        }
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
}


void CUIAPostite::on_IDC_QUIT_clicked()
{
    if (getEmplacement() == "null")
    {
       ui->postite->setCurrentIndex(indexAcceuil);
    }
    else
    {
        ui->postite->setCurrentIndex(indexFile);
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
    settings.setValue("color", "white");
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
    QSettings settings("postite.ini", QSettings::IniFormat);
    settings.beginGroup("postite");
    color = settings.value("color").toString();

    if (color == "white"){
        settings.setValue("color", "black");
        color = "black";
        setColor();
    }else{
        settings.setValue("color", "white");
        color = "white";
        setColor();
    }

    settings.endGroup();
    ui->postite->setCurrentIndex(indexMain);
    onTextChanged();
}

bool CUIAPostite::setColor()
{
    if (color == "white")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 255); color: black;");
        ui->VIEWFILEMAKEDOWN->setStyleSheet("background-color: rgb(255, 255, 255); color: black;");
        return true;
    } else if (color == "black")
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(0, 0, 0); color: white");
        ui->VIEWFILEMAKEDOWN->setStyleSheet("background-color: rgb(0, 0, 0); color: white");
        return true;
    } else
    {
        ui->ZONETEXTE->setStyleSheet("background-color: rgb(255, 255, 255); color: black;");
        ui->VIEWFILEMAKEDOWN->setStyleSheet("background-color: rgb(255, 255, 255); color: black;");
        return false;
    }
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

void CUIAPostite::on_IDC_PRINTPDF_clicked()
{
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
    browser->setHtml(ui->VIEWFILEMAKEDOWN->toHtml());  // Charger le HTML stylisé
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

    QTextBrowser *browser = new QTextBrowser();
    browser->setHtml(htmlContent);
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer);

    if (printDialog.exec() == QDialog::Rejected) {
        return;
    }

    browser->print(&printer);
    delete browser;

}

void CUIAPostite::on_IDC_ADDCHECKBOX_clicked()
{
    QString texteAInserer = "- [ ] Texte\n- [ ] Texte";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ADDLIGNE_clicked()
{
    QString texteAInserer = "---";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ADDCITATION_clicked()
{    
    QString texteAInserer = "> Texte";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ADDLISTEPUCE_clicked()
{
    QString texteAInserer = "- Texte\n- Texte";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ADDLISTENUMERO_clicked()
{
    QString texteAInserer = "1. Texte\n2. Texte";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}


void CUIAPostite::on_IDC_ADDLIENINTERNET_clicked()
{
    QString texteAInserer = "[Texte du lien](URL_du_lien)";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);

}

// Gestion tableau

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
        ui->tabonsere->setCurrentIndex(indexOngletFichier);
    }
}

void CUIAPostite::on_IDC_PARAMETREACCEUIL_clicked()
{
    ui->postite->setCurrentIndex(indexPara);
    ui->IDC_RETOUR->setVisible(true);
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
    typeFile = "ab";
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

    ui->VIEWFILEMAKEDOWN->setReadOnly(false);
    ui->VIEWFILEMAKEDOWN->clear();

    QString markdownText = ui->ZONETEXTE->toPlainText();
    QTextDocument* document = ui->VIEWFILEMAKEDOWN->document();
    document->setMarkdown(markdownText);

    if (color == "black"){
        QString html = ui->VIEWFILEMAKEDOWN->toHtml();
        QString styledHtml = R"(<html><head>
                                <style type='text/css'>
                                    table {
                                        border-collapse: collapse;
                                        width: 85%;
                                        font-family: Arial, sans-serif;
                                        font-size: 20px;
                                        justify-self: center;
                                        color: white;
                                      }

                                      th, td {
                                        border: 1px solid white;
                                        text-align: left;
                                        padding: 8px;
                                        text-align: center;
                                      }

                                      th {
                                        background-color: #616161;
                                      }

                                      tr:nth-child(even) {
                                        background-color: #4b4b4b;
                                      }
                                </style>
                                </head><body>)";
        ui->VIEWFILEMAKEDOWN->setHtml(styledHtml+html+"</body>");
    }else{
        QString html = ui->VIEWFILEMAKEDOWN->toHtml();
        QString styledHtml = R"(<html><head>
                                <style type='text/css'>
                                table {
                                        border-collapse: collapse;
                                        width: 85%;
                                        font-family: Arial, sans-serif;
                                        font-size: 20px;
                                        justify-self: center;
                                      }

                                      th, td {
                                        border: 1px solid black;
                                        text-align: left;
                                        padding: 8px;
                                        text-align: center;
                                      }

                                      th {
                                        background-color: #f2f2f2;
                                      }

                                      tr:nth-child(even) {
                                        background-color: #fafafa;
                                      }
                                </style>
                                </head><body>)";
        ui->VIEWFILEMAKEDOWN->setHtml(styledHtml+html+"</body>");
    }

    ui->VIEWFILEMAKEDOWN->setReadOnly(true);
}

void CUIAPostite::on_IDC_3X3_clicked()
{
    insertTableau(3,3);
    onTextChanged();
}


void CUIAPostite::on_IDC_6X6_clicked()
{
    insertTableau(6,6);
    onTextChanged();
}


void CUIAPostite::on_IDC_10X10_clicked()
{
    insertTableau(10,10);
    onTextChanged();
}


void CUIAPostite::on_IDC_OTHER_clicked()
{
    ui->IDC_SPINCOLONE->setValue(0);
    ui->IDC_SPINLIGNE->setValue(0);
    ui->postite->setCurrentIndex(indexTableau);
}

void CUIAPostite::on_IDC_SAVEHTML_clicked()
{

    QString nameFile = QFileDialog::getSaveFileName(
        this,                         // parent (ou nullptr si hors QWidget)
        "Sélectionner un fichier HTML",
        QString(),                    // dossier par défaut (optionnel)
        "Fichiers HTML (*.html *.htm);"
        );

    if (nameFile.isEmpty() == false)
    {
        if (!nameFile.contains(".html")){
            nameFile = nameFile+".html";
        }

        QString contenu = ui->VIEWFILEMAKEDOWN->toHtml();

        QFile file(nameFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << contenu ;
        }
    }
}


void CUIAPostite::on_IDC_INSERECODE_clicked()
{
    QString texteAInserer = "``````";
    QTextCursor curseur = ui->ZONETEXTE->textCursor();
    curseur.insertText(texteAInserer);
    int positionMilieu = curseur.position() - texteAInserer.length() / 2;
    curseur.setPosition(positionMilieu, QTextCursor::MoveAnchor);
    ui->ZONETEXTE->setTextCursor(curseur);
}

void CUIAPostite::on_IDC_BTNEDITVIEW_clicked()
{
    ui->ZONETEXTE->setVisible(true);
    ui->VIEWFILEMAKEDOWN->setVisible(true);
}


void CUIAPostite::on_IDC_BTNEDIT_clicked()
{
    ui->ZONETEXTE->setVisible(true);
    ui->VIEWFILEMAKEDOWN->setVisible(false);
}


void CUIAPostite::on_IDC_BTNVIEW_clicked()
{
    ui->ZONETEXTE->setVisible(false);
    ui->VIEWFILEMAKEDOWN->setVisible(true);
}


void CUIAPostite::on_IDC_MARKGITHUB_clicked()
{
    typeFile = "md";
    QString fileName = "",contenuTextEdit="";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Markdown Github", "Voulez-vous ouvrir un fichier markdown ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        while (fileName == ""){
            fileName = QFileDialog::getOpenFileName(
                this,
                "Sélectionner un fichier Markdown",
                QDir::homePath(), // Dossier de départ, ici le dossier utilisateur
                "Fichiers Markdown (*.md)"
                );
        }
        QFile file(fileName); // fileName : le chemin récupéré avec QFileDialog
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            contenuTextEdit = in.readAll();
            ui->ZONETEXTE->setPlainText(contenuTextEdit);
            file.close();
            ui->postite->setCurrentIndex(indexMain);
            nameFile = fileName;
        } else {
            QMessageBox::critical(this, "Markdown Github", "Impossible d'ouvrir le fichier");
        }

    }
    else
    {
        reply = QMessageBox::question(this, "Markdown Github", "Voulez-vous créer un fichier markdown ?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes){

            fileName = QFileDialog::getSaveFileName(
                this,
                "Créer un fichier Markdown",
                QDir::homePath() + "/nouveau_fichier.md",
                "Fichiers Markdown (*.md)"
                );

            if (!fileName.isEmpty()) {
                // Facultatif : s'assurer que l'extension .md est bien présente
                if (!fileName.endsWith(".md", Qt::CaseInsensitive))
                    fileName += ".md";

                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << "";
                    file.close();
                    nameFile = fileName;
                    ui->postite->setCurrentIndex(indexMain);
                } else {
                    QMessageBox::critical(this, "Github Markdown", "Impossible de créer le fichier !");
                }
            }else{
                QMessageBox::critical(this, "Github Markdown", "Impossible de créer le fichier vide !");
            }

        }
        else{
            QMessageBox::warning(this,"Github Markdown","Votre contenu ne sera pas sauvegardé.");
            ui->ZONETEXTE->clear();
            ui->postite->setCurrentIndex(indexMain);
        }
    }
}



void CUIAPostite::on_IDC_GITHUBACCEUIL_clicked()
{
    on_IDC_MARKGITHUB_clicked();
}


void CUIAPostite::on_IDC_NEWACCEUIL_clicked()
{
    on_IDC_ADDFILEACCEUIL_clicked();
}


void CUIAPostite::on_IDC_PARAMETREMAINACCEUIL_clicked()
{
    ui->postite->setCurrentIndex(indexPara);
    ui->IDC_RETOUR->setVisible(true);
}

void CUIAPostite::on_IDC_CODESOURCE_clicked()
{
    QUrl url("https://github.com/Arrera-Software/Arrera-Postite");
    QDesktopServices::openUrl(url);
}


void CUIAPostite::on_IDC_SITEWEB_clicked()
{
    QUrl url("https://www.arrera-software.fr/");
    QDesktopServices::openUrl(url);
}


void CUIAPostite::on_IDC_APROPOS1_clicked()
{
    pageApropos = 1 ;
    ui->postite->setCurrentIndex(indexApropos);
}


void CUIAPostite::on_IDC_APROPOS2_clicked()
{
    pageApropos = 2;
    ui->postite->setCurrentIndex(indexApropos);
}


void CUIAPostite::on_IDC_EXITAPROPOS_clicked()
{
    if (pageApropos == 1){
        ui->postite->setCurrentIndex(indexFile);
    }else if (pageApropos == 2){
        ui->postite->setCurrentIndex(indexAcceuil);
    } else {
        ui->postite->setCurrentIndex(indexAcceuil);
    }
}

bool CUIAPostite::traitementSocket(const QString& message){
    if (message.contains("read")){
        socket.sendMessage("contenu : "+ui->ZONETEXTE->toPlainText());
    }
    else{
        return false;
    }
}
