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

}


void CUIAPostite::on_IDC_NEW_clicked()
{

}


void CUIAPostite::on_IDC_PARA_clicked()
{

}


void CUIAPostite::on_IDC_QUIT_clicked()
{

}

