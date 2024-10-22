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

void cargarDatosDescomprimidos() {
    Huffman huffman;


    AppData& appData = AppData::getInstance();
    AVL& avl = appData.getAVLTree();


    // Intenta descomprimir y muestra posibles errores
    std::string decompressedData;
    try {
        decompressedData = huffman.descomprimirArchivo("tablaUserComprimir.edd");
        qDebug() << "Descompresión exitosa";
    } catch (const std::exception& e) {
        qDebug() << "Excepción durante la descompresión:" << e.what();
        return;
    }

    qDebug() << QString::fromStdString(decompressedData);

    std::stringstream ss(decompressedData);
    std::string line;

    try {
        while (std::getline(ss, line)) {
            qDebug() << "Procesando línea:" << QString::fromStdString(line);
            std::stringstream lineStream(line);
            std::string email, nombre, apellido, fechaNacimiento, password;
            if (!std::getline(lineStream, email, ',') ||
                !std::getline(lineStream, nombre, ',') ||
                !std::getline(lineStream, apellido, ',') ||
                !std::getline(lineStream, fechaNacimiento, ',') ||
                !std::getline(lineStream, password, ',')) {
                qDebug() << "Error al leer la línea:" << QString::fromStdString(line);
                continue; // Saltar líneas inválidas
            }
            qDebug() << email << "+" << nombre << "+" << apellido << "+" << password;
            avl.insert(email, nombre, apellido, fechaNacimiento, password);
        }
    } catch (const std::exception& e) {
        qDebug() << "Excepción durante la carga de datos:" << e.what();
    }
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
    cargarDatosDescomprimidos();
}

