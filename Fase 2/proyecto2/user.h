#ifndef USER_H
#define USER_H

#include <QDialog>

namespace Ui {
class user;
}

class user : public QDialog
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    ~user();

private slots:
    void on_buscarbtn_clicked();

    void on_logoutbtn_clicked();

private:
    Ui::user *ui;
};

#endif // USER_H
