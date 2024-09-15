#include "user.h"
#include "ui_user.h"
#include "AppData.h"
#include "mainwindow.h"

user::user(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user)
{
    ui->setupUi(this);
}

user::~user()
{
    delete ui;
}

void user::on_buscarbtn_clicked()
{

}


void user::on_logoutbtn_clicked()
{
    MainWindow *ventana = new MainWindow(this);

    // Mostrar la ventana principal y ocultar la ventana de registro
    this->hide();
    ventana->show();
}

