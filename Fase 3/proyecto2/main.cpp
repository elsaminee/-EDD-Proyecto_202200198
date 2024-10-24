#include "mainwindow.h"
#include "Huffman.h"
#include "AVL.h"
#include "AppData.h"

#include <QApplication>


void cargarDatosDescomprimidosUsuarios() {
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

void cargarDatosDescomprimidosAmistades() {
    Huffman huffman;

    AppData& appData = AppData::getInstance();
    ListaAdyacencia& amistades = appData.getGrafo();

    // Intenta descomprimir y muestra posibles errores
    std::string decompressedData;
    try {
        decompressedData = huffman.descomprimirArchivo("amistadesTable.edd");
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
            std::string usuario, amigos;

            if (!std::getline(lineStream, usuario, ':') || !std::getline(lineStream, amigos)) {
                qDebug() << "Error al leer la línea:" << QString::fromStdString(line);
                continue; // Saltar líneas inválidas
            }

            std::stringstream amigosStream(amigos);
            std::string correoAmigo;
            while (std::getline(amigosStream, correoAmigo, ',')) {
                if (!correoAmigo.empty()) {
                    // Agregar la relación de amistad
                    amistades.agregarRelacion(usuario, correoAmigo);
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Excepción durante la carga de datos de amistades:" << e.what();
    }
}

void cargarDatosDescomprimidosSolicitudes() {
    Huffman huffman;

    AppData& appData = AppData::getInstance();
    Stack& solicitudes = appData.getPilaReceptor();
    ListaAmistad& solicitudes2 = appData.getListaEmisor();

    // Intenta descomprimir y maneja posibles errores
    std::string decompressedData;
    try {
        decompressedData = huffman.descomprimirArchivo("solicitudesTable.edd");
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
            std::string emisor, receptor;

            // Leer el formato Emisor:Receptor
            if (!std::getline(lineStream, emisor, ':') || !std::getline(lineStream, receptor)) {
                qDebug() << "Error al leer la línea:" << QString::fromStdString(line);
                continue;  // Saltar líneas inválidas
            }

            // Agregar la relación de solicitud (Emisor -> Receptor)
            solicitudes.push(emisor, receptor);
            solicitudes2.append(emisor,receptor);
        }
    } catch (const std::exception& e) {
        qDebug() << "Excepción durante la carga de datos de solicitudes:" << e.what();
    }
}

int main(int argc, char *argv[])
{
    cargarDatosDescomprimidosUsuarios();
    cargarDatosDescomprimidosAmistades();
    cargarDatosDescomprimidosSolicitudes();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

