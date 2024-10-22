#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "admin.h"
#include "registro.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginbtn_clicked();

    void on_registrobtn_clicked();


    void on_serializadoBtn_clicked();

private:
    Ui::MainWindow *ui;
    admin *ventanaAdmin = nullptr;
    registro *ventanaRegistro = nullptr;
    user *ventanaUser = nullptr;
};
#endif // MAINWINDOW_H
