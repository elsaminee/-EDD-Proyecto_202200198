#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "admin.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginbtn_clicked()
{
    this->hide();
    admin *ventanaAdmin = new admin(this);
    ventanaAdmin->show();
}

