#ifndef CSETTING_H
#define CSETTING_H

#include <QSettings>
#include <QString>
#include <QFileInfo>
#include <QStandardPaths>

class CSetting {
private:
    bool fileOpen;
    bool fileCreated;
    QString emplacementFile;
    QSettings *settings;
    // Methode private
    int checkOS();

public:
    // Constructeur de base
    CSetting();

    // Constructeur avec le fichier
    CSetting(const QString &namesoft) {

        int os = checkOS();
        QString file;

        if (os == 3){
            file = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.config/";
        }

        QFileInfo checkFile(file);

        if (!checkFile.exists()) {
            // Si le fichier n'existe pas, on le crée
            QFile newFile(file);
            if (newFile.open(QIODevice::WriteOnly)) {  // Crée et vide le fichier
                newFile.close();
            }
            fileCreated = true;
        }
        else{
            fileCreated = false;
        }

        settings = new QSettings(file, QSettings::IniFormat);
        fileOpen = true;
    }

    // Méthode pour obtenir l'état de création du fichier
    bool getFileCreated() const;

    // Méthode pour définir une valeur
    bool setValeur(const QString &section, const QString &key, const QString &value);

    // Méthode pour récupérer une valeur
    QString getValeur(const QString &section, const QString &key) const;

    // Méthode pour supprimer une valeur
    bool supprValeur(const QString &section, const QString &key);
};

#endif // CSETTING_H
