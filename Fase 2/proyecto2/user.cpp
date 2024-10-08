#include "user.h"
#include "ui_user.h"
#include "AppData.h"
#include "mainwindow.h"
#include "crearpublicacion.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QPushButton>

user::user(QWidget *parent, const QString& correo)
    : QDialog(parent)
    , ui(new Ui::user)
    , userCorreo(correo)
{

    ui->setupUi(this);

    AppData& appData = AppData::getInstance();
    // Usar getUserDetails para obtener los datos del usuario

    string nombre, apellido, fechaDeNacimiento, password;
    if (appData.getAVLTree().getUserDetails(userCorreo.toStdString(), nombre, apellido, fechaDeNacimiento, password)) {
        ui->nameEdit->setText(QString::fromStdString(nombre));
        ui->apellidoEdit->setText(QString::fromStdString(apellido));
        ui->fechaEdit->setText(QString::fromStdString(fechaDeNacimiento));
        ui->passEdit->setText(QString::fromStdString(password));
        ui->correoEdit->setText(QString::fromStdString(userCorreo.toStdString()));
    } else {
        qDebug() << "Usuario no encontrado.";
    }
    mostrarUsuariosEnTabla();
    mostrarSolicitudesRecibidas();
    mostrarSolicitudesEnviadas();
    guardarPublicacionesEnArbol();
}

user::~user()
{
    delete ui;
}

void user::on_buscarbtn_clicked()
{
    AppData& appData = AppData::getInstance();
    QString search = ui->searchLine->text();

    string nombre, apellido, fechaDeNacimiento, password;
    if (appData.getAVLTree().getUserDetails(search.toStdString(), nombre, apellido, fechaDeNacimiento, password)) {
        ui->nameLine->setText(QString::fromStdString(nombre));
        ui->apellidoLine->setText(QString::fromStdString(apellido));
        ui->fechaLine->setText(QString::fromStdString(fechaDeNacimiento));
        ui->correoLine->setText(QString::fromStdString(search.toStdString()));
    } else {
        qDebug() << "Usuario no encontrado.";
    }
}


void user::on_logoutbtn_clicked()
{
    AppData& appData = AppData::getInstance();

    // Eliminar el árbol de publicaciones antes de hacer logout
    appData.getArbolDePublicaciones().eliminarArbol();

    MainWindow *ventana = new MainWindow(this);

    // Mostrar la ventana principal y ocultar la ventana actual
    this->hide();
    ventana->show();
}


void user::on_modificarBtn_clicked()
{
    AppData& appData = AppData::getInstance();
    AVL& usuarios =appData.getAVLTree();

    QString nombre = ui->nameEdit->text();
    QString apellido = ui->apellidoEdit->text();
    QString correo = ui->correoEdit->text();
    QString password = ui->passEdit->text();
    QString fecha = ui->fechaEdit->text();

    usuarios.modifyUser(userCorreo.toStdString(), correo.toStdString(), nombre.toStdString(), apellido.toStdString(), fecha.toStdString(), password.toStdString());

    userCorreo = correo;

    this->close();

    MainWindow *ventana = new MainWindow(this);
    ventana->show();
}


void user::on_eliminarBtn_clicked()
{
    AppData& appData = AppData::getInstance();
    AVL& usuarios =appData.getAVLTree();

    usuarios.deleteNode(userCorreo.toStdString());

    this->close();

    MainWindow *ventana = new MainWindow(this);
    ventana->show();
}


void user::on_creaPubliBtn_clicked()
{
    crearPublicacion *ventana = new crearPublicacion(this, userCorreo);
    //No se ocultara la ventana
    ventana->show();

}



void user::on_fechaBtn_clicked()
{
    mostrarPublicacionesEnScrollArea();
}

// Método para mostrar las publicaciones en el QScrollArea
void user::mostrarPublicacionesEnScrollArea() {
    // Limpiar el contenido anterior del QScrollArea
    QVBoxLayout* layout = new QVBoxLayout();

    AppData& appData = AppData::getInstance();
    ArbolBinario& arbolBinario = appData.getArbolDePublicaciones();

    // Recorrer todas las publicaciones del árbol binario
    arbolBinario.recorrerPublicaciones([=](const string& fecha, NodoPublicacion* publicacion) {
        while (publicacion) {
            // Crear un QLabel para mostrar el correo
            QLabel* correoLabel = new QLabel(QString::fromStdString("Correo: " + publicacion->correo));
            qDebug() << "Correo: " + publicacion->correo;
            layout->addWidget(correoLabel);

            // Crear un QLabel para mostrar el contenido de la publicación
            QLabel* contenidoLabel = new QLabel(QString::fromStdString("Contenido: " + publicacion->contenido));
            layout->addWidget(contenidoLabel);

            // Crear un QLabel para mostrar la fecha y hora
            QLabel* fechaHoraLabel = new QLabel(QString::fromStdString("Fecha: " + fecha + " - Hora: " + publicacion->hora));
            layout->addWidget(fechaHoraLabel);

            // Si hay imagen, mostrarla
            if (!publicacion->imagen.empty()) {
                QLabel* imagenLabel = new QLabel();
                QPixmap pixmap(QString::fromStdString(publicacion->imagen));
                imagenLabel->setPixmap(pixmap.scaled(100, 100)); // Redimensionar si es necesario
                layout->addWidget(imagenLabel);
            }

            // Avanzar a la siguiente publicación
            publicacion = publicacion->siguiente;
        }
    });

    // Crear un widget para contener el layout
    QWidget* contentWidget = new QWidget();
    contentWidget->setLayout(layout);

    // Asignar el widget de contenido al QScrollArea
    ui->scrollPubli->setWidget(contentWidget);
}


// Método para mostrar los usuarios en el QTableWidget
void user::mostrarUsuariosEnTabla() {
    // Limpiar el contenido anterior de la tabla
    AppData& appData = AppData::getInstance();
    AVL& arbolAVL = appData.getAVLTree();
    ui->tableUsuario->setRowCount(0);  // Esto elimina todas las filas anteriores

    // Configurar las columnas (Nombre, Apellido, Correo, Fecha de Nacimiento, y el botón de solicitud)
    ui->tableUsuario->setColumnCount(5);
    QStringList headers = {"Nombre", "Apellido", "Correo", "Fecha de Nacimiento", "Acción"};
    ui->tableUsuario->setHorizontalHeaderLabels(headers);

    // Obtener la lista de amistad
    ListaAmistad& listaAmistad = appData.getListaEmisor(); // Asegúrate de que tengas un método para acceder a la lista

    // Recorrer el AVL e insertar cada usuario en una fila de la tabla
    arbolAVL.inorderTraversal([=](std::shared_ptr<Node> usuario) {
        // Verificar si el correo del usuario actual es diferente al correo en el AVL
        if (usuario->email != userCorreo.toStdString() &&
            !listaAmistad.existeSolicitud(userCorreo.toStdString(), usuario->email)) { // Verifica si ya hay una solicitud
            int row = ui->tableUsuario->rowCount();
            ui->tableUsuario->insertRow(row);

            // Crear celdas para los detalles del usuario
            ui->tableUsuario->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(usuario->nombre)));
            ui->tableUsuario->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(usuario->apellido)));
            ui->tableUsuario->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(usuario->email)));
            ui->tableUsuario->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(usuario->fecha_de_nacimiento)));

            // Crear un botón para enviar solicitudes
            QPushButton* btnSolicitud = new QPushButton("Enviar Solicitud");
            ui->tableUsuario->setCellWidget(row, 4, btnSolicitud);

            // Conectar el botón con una función para manejar la solicitud
            connect(btnSolicitud, &QPushButton::clicked, this, [=]() {
                enviarSolicitud(usuario->email, userCorreo.toStdString());  // Llama a una función para enviar solicitud
            });
        }
    });
}

void user::enviarSolicitud(const std::string& receptor, const std::string& correoEmisor) {
    // Aquí puedes implementar la lógica para enviar la solicitud
    QMessageBox::information(this, "Solicitud Enviada", QString::fromStdString("Se ha enviado una solicitud a: " + receptor));
    AppData& appData = AppData::getInstance();
    ListaAmistad& listaSimple = appData.getListaEmisor();
    Stack& pila = appData.getPilaReceptor();

    listaSimple.append(correoEmisor, receptor);
    pila.push(correoEmisor, receptor);

    mostrarUsuariosEnTabla();
    mostrarSolicitudesEnviadas();


}

void user::mostrarSolicitudesRecibidas() {
    ui->tableRecibidas->setRowCount(0);  // Elimina todas las filas existentes en la tabla

    AppData& appData = AppData::getInstance();
    Stack& stackSolicitudes = appData.getPilaReceptor();  // Obtén la pila de solicitudes
    nodoEmisor* emisores = stackSolicitudes.retorno_emisoresDe(userCorreo.toStdString());  // Obtener emisores que enviaron solicitud a userCorreo

    int row = 0;
    while (emisores != nullptr) {
        ui->tableRecibidas->insertRow(row);

        // Agregar emisor a la tabla
        ui->tableRecibidas->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(emisores->emisor)));

        // Botón de aceptar solicitud
        QPushButton* btnAceptar = new QPushButton("Aceptar");
        connect(btnAceptar, &QPushButton::clicked, this, [this, emisores]() {
            aceptarSolicitud(emisores->emisor);
        });
        ui->tableRecibidas->setCellWidget(row, 1, btnAceptar);

        // Botón de rechazar solicitud
        QPushButton* btnRechazar = new QPushButton("Rechazar");
        connect(btnRechazar, &QPushButton::clicked, this, [this, emisores]() {
            rechazarSolicitud(emisores->emisor);
        });
        ui->tableRecibidas->setCellWidget(row, 2, btnRechazar);

        // Avanzar al siguiente emisor
        emisores = emisores->next;
        row++;
    }
}


void user::aceptarSolicitud(const string& emisor) {
    AppData& appData = AppData::getInstance();
    Stack& stackSolicitudes = appData.getPilaReceptor();
    ListaAmistad& solicitudesEmisor = appData.getListaEmisor();

    // Elimina la solicitud de la pila
    stackSolicitudes.removeNode(emisor, userCorreo.toStdString());
    solicitudesEmisor.deleteNode(emisor, userCorreo.toStdString());

    // Aquí podrías agregar la relación de amistad a una lista de amigos
    //appData.getListaDeAmigos().append(emisor, userCorreo.toStdString());

    // Actualiza la tabla
    mostrarSolicitudesRecibidas();
    mostrarUsuariosEnTabla();
}

void user::rechazarSolicitud(const string& emisor) {
    AppData& appData = AppData::getInstance();
    Stack& stackSolicitudes = appData.getPilaReceptor();
    ListaAmistad& solicitudesEmisor = appData.getListaEmisor();

    // Elimina la solicitud de la pila
    stackSolicitudes.removeNode(emisor, userCorreo.toStdString());
    solicitudesEmisor.deleteNode(emisor, userCorreo.toStdString());

    // Actualiza la tabla
    mostrarSolicitudesRecibidas();
    mostrarUsuariosEnTabla();
}


void user::mostrarSolicitudesEnviadas() {
    ui->tableEnviadas->setRowCount(0);  // Elimina todas las filas existentes

    AppData& appData = AppData::getInstance();
    ListaAmistad& listaAmistad = appData.getListaEmisor();  // Obtén la lista de solicitudes

    node_amistad* temp = listaAmistad.getHead();  // Obtener el head de la lista
    int row = 0;

    // Validar si la lista no está vacía
    if (temp == nullptr) {
        qDebug() << "La lista de amistades está vacía.";
        return;  // No hay datos para mostrar
    }

    // Recorrer la lista de solicitudes
    while (temp != nullptr) {
        qDebug() << "Emisor: " << QString::fromStdString(temp->emisor)
        << ", Receptor: " << QString::fromStdString(temp->receptor);

        if (temp->emisor == userCorreo.toStdString()) {  // Solo solicitudes donde el emisor es userCorreo
            ui->tableEnviadas->insertRow(row);

            // Columna 1: Mostrar el correo del receptor
            ui->tableEnviadas->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(temp->receptor)));

            // Columna 2: Botón para cancelar la solicitud
            QPushButton* btnCancelar = new QPushButton("Cancelar");
            connect(btnCancelar, &QPushButton::clicked, this, [this, temp]() {
                cancelarSolicitud(temp->receptor);
            });
            ui->tableEnviadas->setCellWidget(row, 1, btnCancelar);

            row++;
        }
        temp = temp->next;
    }

    if (row == 0) {
        qDebug() << "No hay solicitudes enviadas por el usuario actual.";
    }
}

void user::cancelarSolicitud(const string& receptor) {
    AppData& appData = AppData::getInstance();
    ListaAmistad& listaAmistad = appData.getListaEmisor();
    Stack& stackSolicitudes = appData.getPilaReceptor();

    // Elimina la solicitud de la lista
    stackSolicitudes.removeNode(userCorreo.toStdString(), receptor);
    listaAmistad.deleteNode(userCorreo.toStdString(), receptor);

    // Actualiza la tabla de solicitudes enviadas
    mostrarSolicitudesEnviadas();
    mostrarUsuariosEnTabla();
}

void user::guardarPublicacionesEnArbol() {
    // Supongamos que ya tienes una instancia de la lista de publicaciones
    // Y que también tienes la instancia del árbol binario
    AppData& appData = AppData::getInstance();
    DoublyLinkedList& listaPublicaciones = appData.getListaDoble();
    ArbolBinario& arbolPublicaciones = appData.getArbolDePublicaciones();

    listaPublicaciones.agregarPublicacionesAlArbol(userCorreo.toStdString(), arbolPublicaciones);
}
