#include "crearpublicacion.h"
#include "ui_crearpublicacion.h"
#include <QFileDialog>
#include <QMessageBox>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "AppData.h"

crearPublicacion::crearPublicacion(QWidget *parent, const QString& correo)
    : QDialog(parent)
    , ui(new Ui::crearPublicacion)
    , userCorreo(correo)
{
    ui->setupUi(this);
}

crearPublicacion::~crearPublicacion()
{
    delete ui;
}

void crearPublicacion::graficarBlockchain() {
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


void crearPublicacion::on_seleccionarImagenBtn_clicked()
{
    // Abrir el cuadro de diálogo para seleccionar imagen
    QString fileName = QFileDialog::getOpenFileName(this, "Seleccionar Imagen", "", "Imagenes (*.png *.jpg *.jpeg)");

    // Verificar si se seleccionó una imagen
    if (!fileName.isEmpty()) {
        // Mostrar la ruta de la imagen en un QLineEdit
        ui->imagenPathLine->setText(fileName);
    } else {
        QMessageBox::warning(this, "Error", "No se seleccionó ninguna imagen.");
    }

    QString imagenPath = ui->imagenPathLine->text();
    // Mostrar la imagen en el QLabel
    QPixmap imagen(imagenPath);
    if (!imagen.isNull()) {
        ui->imagenLabel->setPixmap(imagen.scaled(ui->imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen.");
    }

    QMessageBox::information(this, "Publicación Agregada", "La publicación ha sido agregada correctamente.");


}


void crearPublicacion::on_agregarPublicacionBtn_clicked()
{
    AppData& appData = AppData::getInstance();
    ArbolBinario& arbolPublicaciones = appData.getArbolDePublicaciones();
    DoublyLinkedList& publicacionGlobal = appData.getListaDoble();

    // Obtener los datos de la publicación
    QString fechaPublicacion = ui->fechaPublicacionEdit->text();  // Suponiendo que tienes un QLineEdit para la fecha
    QString horaPublicacion = ui->horaPublicacionEdit->text();    // Suponiendo que tienes un QLineEdit para la hora
    QString contenido = ui->contenidoEdit->text();
    QString imagenPath = ui->imagenPathLine->text();              // Para la imagen, si es necesaria

    if (fechaPublicacion.isEmpty() || horaPublicacion.isEmpty() || contenido.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe llenar todos los campos de la publicación.");
        return;
    }

    // Agregar la publicación al árbol binario, incluyendo la hora
    arbolPublicaciones.insertarPublicacion(
        fechaPublicacion.toStdString(),
        userCorreo.toStdString(),
        contenido.toStdString(),
        horaPublicacion.toStdString(),  // Pasamos la hora como parámetro
        imagenPath.toStdString()
        );

    publicacionGlobal.append(
        userCorreo.toStdString(),
        contenido.toStdString(),
        fechaPublicacion.toStdString(),
        horaPublicacion.toStdString(),
        imagenPath.toStdString()
        );
    QMessageBox::information(this, "Publicación Agregada", "La publicación ha sido agregada correctamente.");

    publicacionGlobal.sendToBlockchain(publicacionGlobal);
    graficarBlockchain();



}



void crearPublicacion::on_cerrarBtn_clicked()
{
    this->close();
}

