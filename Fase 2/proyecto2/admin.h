#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "AVL.h"
#include <memory>

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    void eliminarUsuario(shared_ptr<Node> node);
    ~admin();

private slots:
    void on_cargarUserbtn_clicked();

    void on_cargarSolibtn_clicked();

    void on_cargarPublibtn_clicked();

    void on_pushButton_6_clicked();

    void on_showUserbtn_clicked();

    void on_buscarbtn_clicked();

    void mostrarDatosEnTabla();

    void on_ordenbtn_clicked();

private:
    Ui::admin *ui;
    AVL* avlTemporal;
};

#endif // ADMIN_H
