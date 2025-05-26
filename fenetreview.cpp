#include "fenetreview.h"
#include "ui_fenetreview.h"

fenetreView::fenetreView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::fenetreView)
{
    ui->setupUi(this);
}

bool fenetreView::updateText(QString text){
    if (!text.isEmpty()){
        ui->VIEWFILEMAKEDOWN->setHtml(text);
        return true;
    }else {
        return false;
    }
}

fenetreView::~fenetreView()
{
    delete ui;
}

void fenetreView::closeEvent(QCloseEvent *event){
    emit closeSignal();
    QWidget::close();
}

