#ifndef USER_H
#define USER_H

#include <QDialog>

struct NodoPublicacion;

namespace Ui {
class user;
}

class user : public QDialog
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr, const QString& correo = "");
    ~user();
    void mostrarPublicacionesEnScrollArea();  // Método para mostrar publicaciones
    void mostrarUsuariosEnTabla();
    void enviarSolicitud(const std::string& email, const std::string& emailEmisor);
    void mostrarSolicitudesRecibidas();
    void aceptarSolicitud(const std::string& emisor);
    void rechazarSolicitud(const std::string& emisor);
    void cancelarSolicitud(const std::string& receptor);
    void mostrarSolicitudesEnviadas();
    void guardarPublicacionesEnArbol();
    void mostrarComentariosEnDialog(NodoPublicacion* publicacion);  // Recibe la publicación como parámetro
    void llenarComboBoxConFechas();
    void mostrarPublicacionesPorFecha(const QString& fechaSeleccionada);
    void mostrarPublicacionesConOrden(const QString& orden, int cantidadMaxima);
    void mostrarTopFechasConMasPublicaciones();
    void mostrarTopUsuariosConMasComentarios();


private slots:
    void on_buscarbtn_clicked();

    void on_logoutbtn_clicked();

    void on_modificarBtn_clicked();

    void on_eliminarBtn_clicked();

    void on_creaPubliBtn_clicked();

    void on_fechaBtn_clicked();



    void on_AllPubliBtn_2_clicked();

    void on_recorridoBtn_clicked();

    void on_reporteUser_btn_clicked();

    void actualizarPanelConImagen(const QString& imagePath);

    void on_reporteUser_btn_2_clicked();

private:
    Ui::user *ui;
    QString userCorreo;
};

#endif // USER_H
