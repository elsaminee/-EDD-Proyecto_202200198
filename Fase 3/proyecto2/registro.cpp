#include "registro.h"
#include "ui_registro.h"
#include "mainwindow.h"
#include "AppData.h"
#include <QMessageBox>
#include <QCryptographicHash>

registro::registro(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registro)
{
    ui->setupUi(this);
}

registro::~registro()
{
    delete ui;
}

void registro::on_loginbtn_clicked()
{
    MainWindow *ventana = new MainWindow(this);

    // Mostrar la ventana principal y ocultar la ventana de registro
    this->hide();
    ventana->show();
}


void registro::on_registrobtn_clicked()
{
    AppData& appData = AppData::getInstance();
    QString nombre = ui->nameEdit->text();
    QString apellido = ui->apellidoEdit->text();
    QString correo = ui->correoEdit->text();
    QString password = ui->passwordEdit->text();
    QString fecha = ui->fechaEdit->text();

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashPass = hash.result().toHex();

    appData.getAVLTree().insert(correo.toStdString(), nombre.toStdString(), apellido.toStdString(), fecha.toStdString(), hashPass.toStdString());
    QMessageBox::information(this, "Registro exitoso", "¡" + nombre + "  ha sido registrado correctamente!");

}

