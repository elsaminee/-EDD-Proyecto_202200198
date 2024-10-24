#include "dialogmodificar.h"
#include "ui_dialogmodificar.h"

DialogModificar::DialogModificar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogModificar)
{
    ui->setupUi(this);
}

DialogModificar::~DialogModificar()
{
    delete ui;
}

// Setters
void DialogModificar::setNombre(const QString &nombre) {
    ui->lineEditNombre->setText(nombre);
}

void DialogModificar::setApellido(const QString &apellido) {
    ui->lineEditApellido->setText(apellido);
}

void DialogModificar::setEmail(const QString &email) {
    ui->lineEditEmail->setText(email);
}

void DialogModificar::setFechaNacimiento(const QString &fecha) {
    ui->lineEditFechaNacimiento->setText(fecha);
}

void DialogModificar::setPassword(const QString &password) {
    ui->lineEditPassword->setText("password");
}

// Getters
QString DialogModificar::getNombre() const {
    return ui->lineEditNombre->text();
}

QString DialogModificar::getApellido() const {
    return ui->lineEditApellido->text();
}

QString DialogModificar::getEmail() const {
    return ui->lineEditEmail->text();
}

QString DialogModificar::getFechaNacimiento() const {
    return ui->lineEditFechaNacimiento->text();
}

QString DialogModificar::getPassword() const {
    return ui->lineEditPassword->text();

}
