#include "windowsupdate.h"
#include "ui_windowsupdate.h"

WindowsUpdate::WindowsUpdate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WindowsUpdate)
{
    ui->setupUi(this);
}

WindowsUpdate::~WindowsUpdate()
{
    delete ui;
}
