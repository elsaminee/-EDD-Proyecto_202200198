#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "admin.h"
#include "registro.h"
#include "user.h"
#include "AppData.h"
#include "Huffman.h"
#include <QCryptographicHash>


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

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashPass = hash.result().toHex();

    // Verificación de administrador
    if(userAd == usuario.toStdString() && passAd == password.toStdString()) {
        if(!ventanaAdmin){
            ventanaAdmin = new admin(this);
        }
        this->hide();
        ventanaAdmin->show();
    }
    // Verificación de usuario común en el árbol AVL
    else if(AppData::getInstance().getAVLTree().verifyCredentials(usuario.toStdString(), hashPass.toStdString())) {
        if(!ventanaUser){
            ventanaUser = new user(this, usuario);
        }
        this->hide(); // Esconde la ventana de login
        ventanaUser->show();
    } else {
        qDebug() << "Usuario o contraseña incorrecta";
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


void MainWindow::on_serializadoBtn_clicked()
{

}

