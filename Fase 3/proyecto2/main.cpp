#include "mainwindow.h"
#include "Huffman.h"
#include "AVL.h"
#include "AppData.h"

#include <QApplication>


/*
void cargarDatosDescomprimidos() {
    Huffman huffman;
    qDebug() << "Abriendo archivo user.edd";
    std::ifstream inFile("user.edd", std::ios::binary);
    if (!inFile) {
        qDebug() << "No se pudo abrir el archivo user.edd";
        return; // Termina la función si no se puede abrir
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string compressedData = buffer.str();

    AppData& appData = AppData::getInstance();
    AVL& avl =appData.getAVLTree();

    std::string decompressedData = huffman.descomprimir(compressedData);
    std::stringstream ss(decompressedData);
    std::string line;

    try {
        while (std::getline(ss, line)) {
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

            avl.insert(email, nombre, apellido, fechaNacimiento, password);
        }
    } catch (const std::exception& e) {
        qDebug() << "Excepción durante la carga de datos:" << e.what();
    }
}
*/

int main(int argc, char *argv[])
{
    //cargarDatosDescomprimidos();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

