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
#include "dialogmodificar.h"
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QCryptographicHash>
#include "blockchain.h"


admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin)
{
    ui->setupUi(this);
    avlTemporal = new AVL();

}

admin::~admin()
{
    delete ui;
    delete avlTemporal;
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
                    //Si encuentra lo que le pedimos
                    QString nombres = obj.value("nombres").toString();
                    QString apellidos = obj.value("apellidos").toString();
                    QString fechaDeNacimiento = obj.value("fecha_de_nacimiento").toString();
                    QString correo = obj.value("correo").toString();
                    QString password = obj.value("contraseña").toString();
                    QCryptographicHash hash(QCryptographicHash::Sha256);
                    hash.addData(password.toUtf8());
                    QString hashPass = hash.result().toHex();

                    // Insertar el correo en el AVL a través de AppData
                    appData.getAVLTree().insert(correo.toStdString(),nombres.toStdString(), apellidos.toStdString(), fechaDeNacimiento.toStdString(), hashPass.toStdString());
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
    AppData& appData = AppData::getInstance();
    Stack& pila = appData.getPilaReceptor();
    ListaAmistad& emisorLista = appData.getListaEmisor();
    ListaAdyacencia& grafo = appData.getGrafo();

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

                    // Leer los valores de emisor, receptor y estado
                    QString emisor = obj.value("emisor").toString();
                    QString receptor = obj.value("receptor").toString();
                    QString estado = obj.value("estado").toString();

                    // Mostrar los valores leídos (por ahora, omitimos guardarlos)
                    qDebug() << "Emisor: " << emisor;
                    qDebug() << "Receptor: " << receptor;
                    qDebug() << "Estado: " << estado;

                    if ( estado == "PENDIENTE"){
                        qDebug() << "Se acepto la solicitud, ingresando a pila receptor";

                        pila.push(emisor.toStdString(), receptor.toStdString());

                        qDebug() << "Se acepto la solicitud, ingresando a lista emisor";
                        emisorLista.append(emisor.toStdString(), receptor.toStdString());

                    }if(estado == "ACEPTADA"){
                        qDebug() << "Se a aceptado la solicitud.";
                        grafo.agregarRelacion(emisor.toStdString(), receptor.toStdString());

                    }if(estado == "RECHAZADA"){
                        qDebug() << "Se a rechazado la solicitud.";
                    }
                }

                QMessageBox::information(this, "Éxito", "El archivo JSON se leyó correctamente.");
            } else {
                QMessageBox::warning(this, "Error", "El archivo JSON no es un array.");
            }
        } else {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        }
    }
}


void admin::on_cargarPublibtn_clicked()
{
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
                QJsonArray publicacionesArray = doc.array();

                // Acceder a la instancia global de AppData
                AppData& appData = AppData::getInstance();

                // Acceder a la lista doblemente enlazada desde AppData
                DoublyLinkedList& publicaciones = appData.getListaDoble();

                // Iterar sobre el array de publicaciones
                foreach (const QJsonValue &v, publicacionesArray) {
                    QJsonObject publicacionObj = v.toObject();

                    // Leer los datos de la publicación
                    QString correo = publicacionObj.value("correo").toString();
                    QString contenido = publicacionObj.value("contenido").toString();
                    QString fecha = publicacionObj.value("fecha").toString();
                    QString hora = publicacionObj.value("hora").toString();

                    // Insertar la publicación en la lista desde AppData
                    publicaciones.append(correo.toStdString(), contenido.toStdString(), fecha.toStdString(), hora.toStdString());
                    int pubId = publicaciones.getNextId() - 1; // El ID de la última publicación agregada

                    qDebug() << "Publicación de: " << correo;
                    qDebug() << "Contenido: " << contenido;
                    qDebug() << "Fecha: " << fecha << " Hora: " << hora;

                    // Procesar comentarios (si hay)
                    if (publicacionObj.contains("comentarios") && publicacionObj["comentarios"].isArray()) {
                        QJsonArray comentariosArray = publicacionObj["comentarios"].toArray();

                        if (comentariosArray.isEmpty()) {
                            qDebug() << "Esta publicación no tiene comentarios.";
                        } else {
                            foreach (const QJsonValue &commentValue, comentariosArray) {
                                QJsonObject comentarioObj = commentValue.toObject();

                                QString comentarioCorreo = comentarioObj.value("correo").toString();
                                QString comentarioTexto = comentarioObj.value("comentario").toString();


                                qDebug() << "Comentario de: " << comentarioCorreo;
                                qDebug() << "Comentario: " << comentarioTexto;

                                // Agregar el comentario a la publicación correspondiente en la lista desde AppData
                                publicaciones.addComment(pubId, comentarioCorreo.toStdString(), comentarioTexto.toStdString());
                            }
                        }
                    }

                    qDebug() << "---------------------------------------------";
                }

                //Dentroooooooooooooooo

                publicaciones.sendToBlockchain(publicaciones);


                QMessageBox::information(this, "Éxito", "El archivo JSON se leyó correctamente.");
            } else {
                QMessageBox::warning(this, "Error", "El archivo JSON no es un array.");
            }
        } else {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        }
    }

}



void admin::on_pushButton_6_clicked()
{
    //BOTON CERRAR SESION
    MainWindow *ventana = new MainWindow(this);

    // Mostrar la ventana principal y ocultar la ventana de registro
    this->close();
    ventana->show();
}


void admin::on_showUserbtn_clicked()
{
    //InOrder MOSTRAR EN CONSOLA
    AppData& appData = AppData::getInstance();
    appData.getAVLTree().inorder();
}

void admin::mostrarDatosEnTabla() {
    ui->tableBuscar->setRowCount(0);  // Limpiar la tabla antes de mostrar los nuevos datos
    int row = 0;

    AppData& appData = AppData::getInstance();

    // Función lambda para llenar la tabla (inorder traversal)
    auto inorderToTable = [&](shared_ptr<Node> node, auto&& inorderToTableRef) -> void {
        if (!node) return;

        inorderToTableRef(node->left, inorderToTableRef);  // Recorrer subárbol izquierdo

        // Insertar nueva fila en la tabla
        ui->tableBuscar->insertRow(row);

        // Llenar datos
        ui->tableBuscar->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(node->nombre)));
        ui->tableBuscar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(node->apellido)));
        ui->tableBuscar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(node->email)));
        ui->tableBuscar->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(node->fecha_de_nacimiento)));

        // Añadir botones de modificar y eliminar
        QPushButton *btnModificar = new QPushButton("Modificar");
        QPushButton *btnEliminar = new QPushButton("Eliminar");

        connect(btnModificar, &QPushButton::clicked, [this, node, &appData]() {  // Capturar appData
            this->hide();  // Ocultar la ventana admin mientras se abre el diálogo de modificar
            DialogModificar dialog;

            // Establecer los valores actuales del nodo en el formulario
            dialog.setNombre(QString::fromStdString(node->nombre));
            dialog.setApellido(QString::fromStdString(node->apellido));
            dialog.setEmail(QString::fromStdString(node->email));
            dialog.setFechaNacimiento(QString::fromStdString(node->fecha_de_nacimiento));
            dialog.setPassword(QString::fromStdString(node->password));

            // Mostrar el diálogo de modificación
            int resultado = dialog.exec();

            this->show();  // Mostrar nuevamente la ventana admin

            if (resultado == QDialog::Accepted) {
                std::string nuevoNombre = dialog.getNombre().toStdString();
                std::string nuevoApellido = dialog.getApellido().toStdString();
                std::string nuevoEmail = dialog.getEmail().toStdString();
                std::string nuevaFecha = dialog.getFechaNacimiento().toStdString();
                std::string nuevaPassword = dialog.getPassword().toStdString();

                QCryptographicHash hash(QCryptographicHash::Sha256);
                hash.addData(nuevaPassword);
                QString hashPass = hash.result().toHex();

                // Eliminar el nodo original del AVL global
                appData.getAVLTree().deleteNode(node->email);

                // Insertar el nodo con los nuevos valores en el AVL global
                qDebug () << "Password modificada" << hashPass;
                appData.getAVLTree().insert(nuevoEmail, nuevoNombre, nuevoApellido, nuevaFecha, hashPass.toStdString());

                mostrarDatosEnTabla();  // Reflejar cambios
                QMessageBox::information(this, "Modificado", "El usuario ha sido modificado con éxito.");
            }
        });

        connect(btnEliminar, &QPushButton::clicked, [this, node]() {
            this->eliminarUsuario(node);  // Lógica de eliminar usuario
        });

        ui->tableBuscar->setCellWidget(row, 4, btnModificar);
        ui->tableBuscar->setCellWidget(row, 5, btnEliminar);

        row++;  // Avanzar a la siguiente fila

        inorderToTableRef(node->right, inorderToTableRef);  // Recorrer subárbol derecho
    };

    // Ejecutar el recorrido inorder en el AVL global
    inorderToTable(appData.getAVLTree().getRoot(), inorderToTable);
}

void admin::eliminarUsuario(shared_ptr<Node> node) {
    AppData& appData = AppData::getInstance();

    // Eliminar del AVL global en AppData
    appData.getAVLTree().deleteNode(node->email);

    // Actualizar la tabla después de la eliminación
    mostrarDatosEnTabla();

    QMessageBox::information(this, "Eliminado", "El usuario ha sido eliminado.");
}

void admin::on_buscarbtn_clicked() {
    AppData& appData = AppData::getInstance();

    QString correo = ui->searchLine->text();
    std::string email = correo.toStdString();

    // Limpiar la tabla antes de mostrar los datos
    ui->tableBuscar->setRowCount(0);
    int row = 0;

    // Buscar en el AVL global
    shared_ptr<Node> nodo = appData.getAVLTree().getNode(email);
    if (nodo) {
        // Si lo encuentra, obtenemos los datos y los mostramos en la tabla
        ui->tableBuscar->insertRow(row);

        // Llenar datos
        ui->tableBuscar->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(nodo->nombre)));
        ui->tableBuscar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(nodo->apellido)));
        ui->tableBuscar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(nodo->email)));
        ui->tableBuscar->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(nodo->fecha_de_nacimiento)));

        // Añadir botones de modificar y eliminar
        QPushButton *btnModificar = new QPushButton("Modificar");
        QPushButton *btnEliminar = new QPushButton("Eliminar");

        // Captura explícita de appData por referencia en la lambda
        connect(btnModificar, &QPushButton::clicked, [this, nodo, &appData]() {
            this->hide();  // Ocultar la ventana admin mientras se abre el diálogo de modificar
            DialogModificar dialog;

            // Establecer los valores actuales del nodo en el formulario
            dialog.setNombre(QString::fromStdString(nodo->nombre));
            dialog.setApellido(QString::fromStdString(nodo->apellido));
            dialog.setEmail(QString::fromStdString(nodo->email));
            dialog.setFechaNacimiento(QString::fromStdString(nodo->fecha_de_nacimiento));
            dialog.setPassword(QString::fromStdString(nodo->password));

            int resultado = dialog.exec();
            this->show();  // Mostrar nuevamente la ventana admin

            if (resultado == QDialog::Accepted) {
                std::string nuevoNombre = dialog.getNombre().toStdString();
                std::string nuevoApellido = dialog.getApellido().toStdString();
                std::string nuevoEmail = dialog.getEmail().toStdString();
                std::string nuevaFecha = dialog.getFechaNacimiento().toStdString();
                std::string nuevaPassword = dialog.getPassword().toStdString();

                // Eliminar el nodo original del AVL global
                appData.getAVLTree().deleteNode(nodo->email);

                // Insertar el nodo con los nuevos valores en el AVL global
                appData.getAVLTree().insert(nuevoEmail, nuevoNombre, nuevoApellido, nuevaFecha, nuevaPassword);

                mostrarDatosEnTabla();
                QMessageBox::information(this, "Modificado", "El usuario ha sido modificado con éxito.");
            }
        });

        // Captura explícita de appData por referencia en la lambda
        connect(btnEliminar, &QPushButton::clicked, [this, nodo, &appData]() {
            this->eliminarUsuario(nodo);  // Lógica de eliminar usuario
        });

        ui->tableBuscar->setCellWidget(row, 4, btnModificar);
        ui->tableBuscar->setCellWidget(row, 5, btnEliminar);

        QMessageBox::information(this, "Búsqueda exitosa", "El usuario ha sido encontrado.");
    } else {
        QMessageBox::warning(this, "Error", "Usuario no encontrado en el AVL global.");
    }
}

void admin::on_ordenbtn_clicked() {
    // Obtener el texto seleccionado del comboBox
    QString ordenSeleccionado = ui->ordenBox->currentText();

    // Limpiar la tabla antes de mostrar los nuevos datos
    ui->tableBuscar->setRowCount(0);
    int row = 0;

    // Función lambda para llenar la tabla en cada tipo de recorrido
    auto addToTable = [&](shared_ptr<Node> node) {
        ui->tableBuscar->insertRow(row);
        ui->tableBuscar->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(node->nombre)));
        ui->tableBuscar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(node->apellido)));
        ui->tableBuscar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(node->email)));
        ui->tableBuscar->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(node->fecha_de_nacimiento)));

        QPushButton *btnModificar = new QPushButton("Modificar");
        QPushButton *btnEliminar = new QPushButton("Eliminar");

        connect(btnModificar, &QPushButton::clicked, [this, node]() {
            this->hide();
            DialogModificar dialog;

            dialog.setNombre(QString::fromStdString(node->nombre));
            dialog.setApellido(QString::fromStdString(node->apellido));
            dialog.setEmail(QString::fromStdString(node->email));
            dialog.setFechaNacimiento(QString::fromStdString(node->fecha_de_nacimiento));
            dialog.setPassword(QString::fromStdString(node->password));

            int resultado = dialog.exec();
            this->show();

            if (resultado == QDialog::Accepted) {
                std::string nuevoNombre = dialog.getNombre().toStdString();
                std::string nuevoApellido = dialog.getApellido().toStdString();
                std::string nuevoEmail = dialog.getEmail().toStdString();
                std::string nuevaFecha = dialog.getFechaNacimiento().toStdString();
                std::string nuevaPassword = dialog.getPassword().toStdString();

                AppData& appData = AppData::getInstance();

                // Eliminar el nodo original del AVL global
                appData.getAVLTree().deleteNode(node->email);

                // Insertar el nodo con los nuevos valores en el AVL global
                appData.getAVLTree().insert(nuevoEmail, nuevoNombre, nuevoApellido, nuevaFecha, nuevaPassword);

                // Actualizar la tabla con los nuevos datos
                mostrarDatosEnTabla();

                QMessageBox::information(this, "Modificado", "El usuario ha sido modificado con éxito.");
            }
        });

        connect(btnEliminar, &QPushButton::clicked, [this, node]() {
            this->eliminarUsuario(node);
        });

        ui->tableBuscar->setCellWidget(row, 4, btnModificar);
        ui->tableBuscar->setCellWidget(row, 5, btnEliminar);
        row++;
    };

    AppData& appData = AppData::getInstance();

    // Dependiendo del valor seleccionado, se ejecuta el tipo de recorrido correspondiente
    if (ordenSeleccionado == "InOrder") {
        appData.getAVLTree().inorderTraversal([&](shared_ptr<Node> node) {
            addToTable(node);
        });
    }
    else if (ordenSeleccionado == "PreOrder") {
        appData.getAVLTree().preorderTraversal([&](shared_ptr<Node> node) {
            addToTable(node);
        });
    }
    else if (ordenSeleccionado == "PostOrder") {
        appData.getAVLTree().postorderTraversal([&](shared_ptr<Node> node) {
            addToTable(node);
        });
    }
}

void admin::on_reporteAdminbtn_clicked()
{
    AppData& appData = AppData::getInstance();

    // Generar el archivo DOT y convertirlo en imagen PNG
    appData.getAVLTree().generateDot();
    appData.getListaDoble().generateDot();

    QString rutaImagen = QDir::currentPath() + "/reportes/avl.png";
    QString rutaImagenPubli = QDir::currentPath() + "/reportes/listaPublicaciones.png";

    // Cargar la imagen
    QPixmap pixmap(rutaImagen);

    // Asegúrate de que el widget sea un QLabel para mostrar la imagen
    QLabel* label = findChild<QLabel*>("avlWidget"); // Debes tener un QLabel llamado "avlWidget"

    if (label) {
        // Redimensionar el QLabel al tamaño de la imagen
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "No se encontró el QLabel llamado 'avlWidget'.";
    }
    //Metodo para crear una imagen en el lado de las publicaciones
    actualizarPanelConImagen(rutaImagenPubli);
}

void admin::actualizarPanelConImagen(const QString& imagePath) {
    // Obtiene el layout existente en el frame
    QLayout* existingLayout = ui->frameScroll->layout();

    // Si existe un layout, lo elimina completamente
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    // Crea un nuevo layout vertical
    QVBoxLayout* newLayout = new QVBoxLayout();
    //Este es el nombre del widget
    ui->widgetPubli->setLayout(newLayout);

    // Crea un label para la imagen
    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap);
    imageLabel->resize(pixmap.size());

    // Crea un scroll area para contener la imagen
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageLabel);

    // Agrega el scroll area al nuevo layout
    newLayout->addWidget(scrollArea);
}


void admin::ImagenAdyacencia(const QString& imagePath) {
    // Obtener el layout existente en el widget "widgetPubli"
    QLayout* existingLayout = ui->widgetPubli_4->layout();

    // Si existe un layout previo, eliminar sus elementos pero no el layout completo
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (item->widget()) {
                delete item->widget();  // Eliminar el widget contenido
            }
            delete item;  // Eliminar el item
        }
    } else {
        // Si no hay un layout, crear uno nuevo y asignarlo
        QVBoxLayout* newLayout = new QVBoxLayout();
        ui->widgetPubli_4->setLayout(newLayout);
        existingLayout = newLayout;  // Asignamos el nuevo layout a existingLayout
    }

    // Crear un QLabel para mostrar la imagen
    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);  // Escalar la imagen al tamaño del QLabel

        // Crear el QScrollArea para contener el QLabel
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);  // Permitir redimensionar
        scrollArea->setWidget(imageLabel);     // Asignar el QLabel al scroll area

        // Añadir el scrollArea al layout existente
        existingLayout->addWidget(scrollArea);
    } else {
        qDebug() << "Error: No se pudo cargar la imagen desde la ruta: " << imagePath;
        QLabel* errorLabel = new QLabel("No se pudo cargar la imagen.");
        existingLayout->addWidget(errorLabel);
    }
}

void admin::ImagenGrafo(const QString& imagePath) {
    // Obtener el layout existente en el widget "widgetPubli"
    QLayout* existingLayout = ui->widgetPubli_5->layout();

    // Si existe un layout previo, eliminar sus elementos pero no el layout completo
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (item->widget()) {
                delete item->widget();  // Eliminar el widget contenido
            }
            delete item;  // Eliminar el item
        }
    } else {
        // Si no hay un layout, crear uno nuevo y asignarlo
        QVBoxLayout* newLayout = new QVBoxLayout();
        ui->widgetPubli_5->setLayout(newLayout);
        existingLayout = newLayout;  // Asignamos el nuevo layout a existingLayout
    }

    // Crear un QLabel para mostrar la imagen
    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);  // Escalar la imagen al tamaño del QLabel

        // Crear el QScrollArea para contener el QLabel
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);  // Permitir redimensionar
        scrollArea->setWidget(imageLabel);     // Asignar el QLabel al scroll area

        // Añadir el scrollArea al layout existente
        existingLayout->addWidget(scrollArea);
    } else {
        qDebug() << "Error: No se pudo cargar la imagen desde la ruta: " << imagePath;
        QLabel* errorLabel = new QLabel("No se pudo cargar la imagen.");
        existingLayout->addWidget(errorLabel);
    }
}

void admin::graficarBlockchainAdmin() {
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

void admin::ImagenBlockChain(const QString& imagePath) {
    // Obtener el layout existente en el widget "widgetPubli"
    QLayout* existingLayout = ui->widgetPubli_7->layout();

    // Si existe un layout previo, eliminar sus elementos pero no el layout completo
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (item->widget()) {
                delete item->widget();  // Eliminar el widget contenido
            }
            delete item;  // Eliminar el item
        }
    } else {
        // Si no hay un layout, crear uno nuevo y asignarlo
        QVBoxLayout* newLayout = new QVBoxLayout();
        ui->widgetPubli_7->setLayout(newLayout);
        existingLayout = newLayout;  // Asignamos el nuevo layout a existingLayout
    }

    // Crear un QLabel para mostrar la imagen
    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);  // Escalar la imagen al tamaño del QLabel

        // Crear el QScrollArea para contener el QLabel
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);  // Permitir redimensionar
        scrollArea->setWidget(imageLabel);     // Asignar el QLabel al scroll area

        // Añadir el scrollArea al layout existente
        existingLayout->addWidget(scrollArea);
    } else {
        qDebug() << "Error: No se pudo cargar la imagen desde la ruta: " << imagePath;
        QLabel* errorLabel = new QLabel("No se pudo cargar la imagen.");
        existingLayout->addWidget(errorLabel);
    }
}


void admin::on_nuevoReporteBtn_clicked()
{
    AppData& appData = AppData::getInstance();
    ListaAdyacencia& lista = appData.getGrafo();

    lista.graph_listaEnlazada();
    lista.graph();
    graficarBlockchainAdmin();

    ImagenAdyacencia("reportes/graph_lista_enlazada.png");
    ImagenGrafo("reportes/graph.png");
    ImagenBlockChain("reportes/bloqueChain.png");

}

