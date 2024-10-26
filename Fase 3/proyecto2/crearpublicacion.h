#ifndef CREARPUBLICACION_H
#define CREARPUBLICACION_H

#include <QDialog>

namespace Ui {
class crearPublicacion;
}

class crearPublicacion : public QDialog
{
    Q_OBJECT

public:
    explicit crearPublicacion(QWidget *parent = nullptr, const QString& correo = "");
    ~crearPublicacion();

private slots:
    void on_seleccionarImagenBtn_clicked();

    void on_agregarPublicacionBtn_clicked();
    void graficarBlockchain();

    void on_cerrarBtn_clicked();

private:
    Ui::crearPublicacion *ui;
    QString userCorreo;
};

#endif // CREARPUBLICACION_H
