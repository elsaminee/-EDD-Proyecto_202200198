#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "admin.h"
#include "registro.h"
#include "user.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginbtn_clicked()
{

    std::string userAd = "admin@gmail.com";
    std::string passAd = "EDD2S2024";

    QString usuario = ui->userline->text();
    QString password = ui->passline->text();

    if(userAd == usuario.toStdString() && passAd == password.toStdString() ){
        if(!ventanaAdmin){
            ventanaAdmin = new admin(this);
        }
        this->hide();
        ventanaAdmin->show();
    }else if(usuario.toStdString() == "user" && password.toStdString() == "user") {
            if(!ventanaUser){
                ventanaUser = new user(this);
                }
            this->hide(); //Ventana login escondiida
            ventanaUser->show();

    }
    else {
        qDebug() << "Usuario o password incorrecta";
    }


}


void MainWindow::on_registrobtn_clicked()
{
    if(!ventanaRegistro){
        ventanaRegistro = new registro(this);
    }
    this->hide();
    ventanaRegistro->show();
}

