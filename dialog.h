#ifndef DIALOG_H
#define DIALOG_H

#include <string>

#include <QtGui/QDialog>

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void accepted(std::string username, std::string password, bool remember);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DialogClass *ui;
};

#endif // DIALOG_H
