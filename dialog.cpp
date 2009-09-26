#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogClass)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
	this->hide();
	emit accepted(ui->username->text().toStdString(), ui->password->text().toStdString(), ui->rememberMe->isChecked());
}

void Dialog::on_buttonBox_rejected()
{
	this->hide();
}
