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

                // Eliminar el nodo original del AVL global
                appData.getAVLTree().deleteNode(node->email);

                // Insertar el nodo con los nuevos valores en el AVL global
                appData.getAVLTree().insert(nuevoEmail, nuevoNombre, nuevoApellido, nuevaFecha, nuevaPassword);

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
