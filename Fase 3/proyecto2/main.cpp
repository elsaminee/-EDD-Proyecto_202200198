#include "mainwindow.h"
#include "Huffman.h"
#include "AVL.h"
#include "AppData.h"

#include <QApplication>
#include <qjsondocument.h>
#include <QJsonArray>
#include <qjsonobject.h>


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

void cargarDatosPublicaciones()
{
    // Usa una ruta relativa basada en el directorio actual
    QString carpeta = QDir::currentPath() + "/bloques/";
    QDir directorio(carpeta);
    qDebug() << "Buscando en:" << carpeta;

    if (!directorio.exists()) {
        qDebug() << "La carpeta 'bloques' no existe en el directorio de ejecución.";
        return;
    }

    // Acceder a la instancia global de AppData
    AppData& appData = AppData::getInstance();
    DoublyLinkedList& publicaciones = appData.getListaDoble();

    int contador = 1;  // Contador para los archivos en orden
    while (true) {
        QString nombreArchivo = carpeta + "block_" + QString::number(contador) + ".json";
        QFile file(nombreArchivo);

        // Si el archivo no existe, termina la búsqueda
        if (!file.exists()) {
            qDebug() << "Archivo no encontrado:" << nombreArchivo;
            break;
        }

        // Si el archivo existe, intenta abrir y procesar su contenido
        if (file.open(QFile::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                QJsonObject blockObj = doc.object();

                // Verifica si contiene "DATA" y si es un array
                if (blockObj.contains("DATA") && blockObj["DATA"].isArray()) {
                    QJsonArray publicacionesArray = blockObj["DATA"].toArray();

                    foreach (const QJsonValue &v, publicacionesArray) {
                        QJsonObject publicacionObj = v.toObject();
                        QString correo = publicacionObj.value("correo").toString();
                        QString contenido = publicacionObj.value("contenido").toString();
                        QString fecha = publicacionObj.value("fecha").toString();
                        QString hora = publicacionObj.value("hora").toString();
                        QString imagen = publicacionObj.value("imagen").toString();

                        publicaciones.append(correo.toStdString(), contenido.toStdString(),
                                             fecha.toStdString(), hora.toStdString(),
                                             imagen.isEmpty() ? std::string() : imagen.toStdString());

                        int pubId = publicaciones.getNextId() - 1;

                        // Procesar comentarios, si existen
                        if (publicacionObj.contains("comentarios") && publicacionObj["comentarios"].isArray()) {
                            QJsonArray comentariosArray = publicacionObj["comentarios"].toArray();
                            foreach (const QJsonValue &commentValue, comentariosArray) {
                                QJsonObject comentarioObj = commentValue.toObject();
                                QString comentarioCorreo = comentarioObj.value("correo").toString();
                                QString comentarioTexto = comentarioObj.value("comentario").toString();
                                publicaciones.addComment(pubId, comentarioCorreo.toStdString(), comentarioTexto.toStdString());
                            }
                        }
                    }
                } else {
                    qDebug() << "El archivo no contiene el array 'DATA':" << nombreArchivo;
                }
            } else {
                qDebug() << "El archivo no contiene un objeto JSON:" << nombreArchivo;
            }
        } else {
            qDebug() << "No se pudo abrir el archivo:" << nombreArchivo;
        }

        contador++;  // Incrementa el contador para buscar el siguiente archivo
    }

    qDebug() << "Carga de archivos JSON completada.";
}

void graficarBlockchain() {
    QString carpeta = QDir::currentPath() + "/bloques/";
    QDir directorio(carpeta);
    qDebug() << "Buscando en:" << carpeta;

    if (!directorio.exists()) {
        qDebug() << "La carpeta 'bloques' no existe en el directorio de ejecución.";
        return;
    }

    QFile archivoDot("reportes/bloqueChain.dot");
    if (!archivoDot.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "No se pudo crear el archivo DOT";
        return;
    }

    QTextStream salida(&archivoDot);
    salida << "digraph G {\n";
    salida << "  node [shape=record];\n";
    salida << " rankdir=LR";

    int contador = 1;
    QString previoHash;

    while (true) {
        QString nombreArchivo = carpeta + "block_" + QString::number(contador) + ".json";
        QFile file(nombreArchivo);

        if (!file.exists()) {
            qDebug() << "Archivo no encontrado:" << nombreArchivo;
            break;
        }

        if (file.open(QFile::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (!doc.isObject()) {
                qDebug() << "Formato JSON inválido en:" << nombreArchivo;
                break;
            }

            QJsonObject blockObj = doc.object();

            int index = blockObj.value("INDEX").toInt();
            QString nonce = blockObj.value("NONCE").toString();
            QString timestamp = blockObj.value("TIMESTAMP").toString();
            QString currentHash = blockObj.value("HASH").toString();
            QString previousHash = blockObj.value("PREVIOUSHASH").toString();

            salida << "  block" << index << " [label=\"";
            salida << "INDEX: " << index << " | TIMESTAMP: " << timestamp << " | NONCE:  " << nonce << " \\n \\n HASH: " << currentHash << "\\n \\n  PREVIOUSHASH: " << previousHash;

            if (blockObj.contains("DATA") && blockObj["DATA"].isArray()) {
                QJsonArray publicacionesArray = blockObj["DATA"].toArray();
                salida << " | ";

                for (const QJsonValue &v : publicacionesArray) {
                    QJsonObject publicacionObj = v.toObject();
                    QString correo = publicacionObj.value("correo").toString();
                    QString contenido = publicacionObj.value("contenido").toString();

                    salida << " Correo: " << correo << " \\n \\n Contenido: " << contenido;

                    if (publicacionObj.contains("comentarios") && publicacionObj["comentarios"].isArray()) {
                        QJsonArray comentariosArray = publicacionObj["comentarios"].toArray();
                        salida << " \n \n Comentarios: [";

                        for (const QJsonValue &commentValue : comentariosArray) {
                            QJsonObject comentarioObj = commentValue.toObject();
                            QString comentarioCorreo = comentarioObj.value("correo").toString();
                            QString comentarioTexto = comentarioObj.value("comentario").toString();
                            salida << " Correo: " << comentarioCorreo << " | Comentario: " << comentarioTexto << " ";
                        }
                        salida << "]";
                    }
                    salida << "  ";
                }
                salida << "]";
            }
            salida << "\"];\n";

            // Enlazar con el bloque anterior
            if (contador > 1) {
                salida << "  block" << contador - 1 << " -> block" << contador << ";\n";
            }

            previoHash = currentHash; // Actualizar el hash previo para el próximo bloque
        } else {
            qDebug() << "No se pudo abrir el archivo:" << nombreArchivo;
        }
        contador++;
    }

    salida << "}\n";
    archivoDot.close();

    // Crear el archivo PNG usando Graphviz
    std::string command = "dot -Tpng reportes/bloqueChain.dot -o reportes/bloqueChain.png";
    if (system(command.c_str()) == -1) {
        qDebug() << "Error al ejecutar el comando Graphviz.";
    } else {
        qDebug() << "Imagen de la blockchain generada exitosamente: bloqueChain.png";
    }
}

int main(int argc, char *argv[])
{
    cargarDatosDescomprimidosUsuarios();
    cargarDatosDescomprimidosAmistades();
    cargarDatosDescomprimidosSolicitudes();
    cargarDatosPublicaciones();
    graficarBlockchain();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
