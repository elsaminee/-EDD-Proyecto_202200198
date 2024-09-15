#include "admin.h"
#include "ui_admin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "AppData.h"
#include "mainwindow.h"

extern AppData appData;

admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}

void admin::on_cargarUserbtn_clicked()
{
    AppData& appData = AppData::getInstance();

    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Open file",
        "/",
        "Text Files (*.json);;All Files (*,*)");


    if (!ruta.isEmpty()) {
        QFile file(ruta);

        if (file.open(QFile::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(jsonData);

            if (doc.isArray()) {
                QJsonArray root = doc.array();

                foreach (const QJsonValue &v, root) {
                    QJsonObject obj = v.toObject();

                    QString nombres = obj.value("nombres").toString();
                    QString apellidos = obj.value("apellidos").toString();
                    QString fechaDeNacimiento = obj.value("fecha_de_nacimiento").toString();
                    QString correo = obj.value("correo").toString();
                    QString password = obj.value("contraseña").toString();

                    // Insertar el correo en el AVL a través de AppData
                    appData.getAVLTree().insert(correo.toStdString(),nombres.toStdString(), apellidos.toStdString(), fechaDeNacimiento.toStdString(), password.toStdString());
                }


                QMessageBox::information(this, "Éxito", "El archivo JSON se leyó y los correos se almacenaron en el árbol AVL.");
            } else {
                QMessageBox::warning(this, "Error", "El archivo JSON no es un array.");
            }
        } else {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        }
    }
}


void admin::on_cargarSolibtn_clicked()
{

}


void admin::on_cargarPublibtn_clicked()
{

}


void admin::on_pushButton_6_clicked()
{
    //BOTON CERRAR SESION
    MainWindow *ventana = new MainWindow(this);

    // Mostrar la ventana principal y ocultar la ventana de registro
    this->hide();
    ventana->show();
}


void admin::on_showUserbtn_clicked()
{
    //InOrder
    AppData& appData = AppData::getInstance();
    appData.getAVLTree().inorder();
}

