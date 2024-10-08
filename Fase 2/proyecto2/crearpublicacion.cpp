#include "crearpublicacion.h"
#include "ui_crearpublicacion.h"
#include <QFileDialog>
#include <QMessageBox>
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



}


void crearPublicacion::on_cerrarBtn_clicked()
{
    this->close();
}

