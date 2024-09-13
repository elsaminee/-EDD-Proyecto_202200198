#include "admin.h"
#include "ui_admin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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

    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Open file",
        "/",
        "Text Files (*.json);;All Files (*,*)");

    QString final;

    if (!ruta.isEmpty()) {
        QFile file(ruta);

        if (file.open(QFile::ReadOnly)) {
            // Leer todo el contenido del archivo
            QByteArray jsonData = file.readAll();
            file.close();

            // Crear un documento JSON a partir de los datos
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);

            // Comprobar si el documento es un array
            if (doc.isArray()) {
                QJsonArray root = doc.array();

                // Iterar a través del array
                foreach (const QJsonValue &v, root) {
                    QJsonObject obj = v.toObject();

                    QString nombres = obj.value("nombres").toString();
                    QString apellidos = obj.value("apellidos").toString();
                    QString fechaDeNacimiento = obj.value("fecha_de_nacimiento").toString();
                    QString correo = obj.value("correo").toString();
                    QString contrasena = obj.value("contraseña").toString();

                    // Concatenar los valores en el formato que desees
                    final += "Nombres: " + nombres + "\n";
                    final += "Apellidos: " + apellidos + "\n";
                    final += "Fecha de nacimiento: " + fechaDeNacimiento + "\n";
                    final += "Correo: " + correo + "\n";
                    final += "Contraseña: " + contrasena + "\n\n";  // \n\n para separación entre usuarios
                }

                // Mostrar el resultado en el widget 'texto'
                ui->texto->setText(final);

                // Mostrar un MessageBox indicando que se leyó correctamente
                QMessageBox::information(this, "Éxito", "El archivo JSON se leyó correctamente.");
            } else {
                // Mostrar un mensaje de error si no es un array
                QMessageBox::warning(this, "Error", "El archivo JSON no es un array.");
            }

        } else {
            // Mostrar un mensaje de error si no se pudo abrir el archivo
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

