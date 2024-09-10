#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "admin.h"


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
    } else {
        qDebug() << "Usuario o password incorrecta";
    }


}

