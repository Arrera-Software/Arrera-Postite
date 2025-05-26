#ifndef FENETREVIEW_H
#define FENETREVIEW_H

#include <QDialog>

namespace Ui {
class fenetreView;
}

class fenetreView : public QDialog
{
    Q_OBJECT

public:
    explicit fenetreView(QWidget *parent = nullptr);
    bool updateText(QString text);
    ~fenetreView();
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::fenetreView *ui;
signals:
    void closeSignal();
};

#endif // FENETREVIEW_H
