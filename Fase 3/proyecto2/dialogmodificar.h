#ifndef DIALOGMODIFICAR_H
#define DIALOGMODIFICAR_H

#include <QDialog>

namespace Ui {
class DialogModificar;
}

class DialogModificar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModificar(QWidget *parent = nullptr);
    ~DialogModificar();

    // Métodos para establecer los valores actuales del nodo
    void setNombre(const QString &nombre);
    void setApellido(const QString &apellido);
    void setEmail(const QString &email);
    void setFechaNacimiento(const QString &fecha);
    void setPassword(const QString &password);

    // Métodos para obtener los nuevos valores ingresados
    QString getNombre() const;
    QString getApellido() const;
    QString getEmail() const;
    QString getFechaNacimiento() const;
    QString getPassword() const;

private:
    Ui::DialogModificar *ui;
};

#endif // DIALOGMODIFICAR_H
