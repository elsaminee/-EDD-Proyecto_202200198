#include "registro.h"
#include "ui_registro.h"
#include "mainwindow.h"

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

