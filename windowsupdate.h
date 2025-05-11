#ifndef WINDOWSUPDATE_H
#define WINDOWSUPDATE_H

#include <QDialog>

namespace Ui {
class WindowsUpdate;
}

class WindowsUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit WindowsUpdate(QWidget *parent = nullptr);
    ~WindowsUpdate();

private:
    Ui::WindowsUpdate *ui;
};

#endif // WINDOWSUPDATE_H
