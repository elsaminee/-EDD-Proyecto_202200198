#ifndef AVL_H
#define AVL_H

#include <memory>
#include <string>
#include <QDebug>

using namespace std;

class Node {
public:
    string email;                  // Correo electrónico como criterio de ordenación
    string nombre;
    string apellido;
    string fecha_de_nacimiento;
    string password;
    int height;                    // Altura del nodo
    shared_ptr<Node> left, right;  // Hijos izquierdo y derecho

    // Constructor que inicializa todos los campos
    Node(const string& mail, const string& name, const string& surname, const string& birthdate, const string& pass)
        : email(mail), nombre(name), apellido(surname), fecha_de_nacimiento(birthdate), password(pass),
        height(1), left(nullptr), right(nullptr) {}
};

class AVL {
private:
    shared_ptr<Node> root;

    // Obtener la altura de un nodo
    int height(shared_ptr<Node> node) {
        return node ? node->height : 0;
    }

    // Obtener el factor de balance de un nodo
    int getBalance(shared_ptr<Node> node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Rotación hacia la derecha
    shared_ptr<Node> rightRotate(shared_ptr<Node> y) {
        shared_ptr<Node> x = y->left;
        shared_ptr<Node> T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Rotación hacia la izquierda
    shared_ptr<Node> leftRotate(shared_ptr<Node> x) {
        shared_ptr<Node> y = x->right;
        shared_ptr<Node> T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Inserción en el árbol AVL
    shared_ptr<Node> insertRec(shared_ptr<Node> node, const string& email, const string& nombre,
                               const string& apellido, const string& fecha_de_nacimiento, const string& password) {
        if (!node) {
            return make_shared<Node>(email, nombre, apellido, fecha_de_nacimiento, password);
        }

        if (email < node->email) {
            node->left = insertRec(node->left, email, nombre, apellido, fecha_de_nacimiento, password);
        } else if (email > node->email) {
            node->right = insertRec(node->right, email, nombre, apellido, fecha_de_nacimiento, password);
        } else {
            return node; // No se permiten duplicados
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && email < node->left->email) {
            return rightRotate(node);
        }
        if (balance < -1 && email > node->right->email) {
            return leftRotate(node);
        }
        if (balance > 1 && email > node->left->email) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && email < node->right->email) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Función para buscar un nodo por correo (searchRec)
    shared_ptr<Node> searchRec(shared_ptr<Node> node, const string& email) const {
        if (!node || node->email == email) {
            return node;
        }

        if (email < node->email) {
            return searchRec(node->left, email);
        }

        return searchRec(node->right, email);
    }

    // Recorrido inorder para mostrar el árbol
    void inorderRec(shared_ptr<Node> node) const {
        if (node) {
            inorderRec(node->left);
            qDebug() << "Correo:" << QString::fromStdString(node->email)
                     << "Nombre:" << QString::fromStdString(node->nombre)
                     << "Apellido:" << QString::fromStdString(node->apellido)
                     << "Fecha de Nacimiento:" << QString::fromStdString(node->fecha_de_nacimiento)
                     << "Password:" << QString::fromStdString(node->password);
            inorderRec(node->right);
        }
    }

public:
    AVL() : root(nullptr) {}

    // Inserción pública
    void insert(const string& email, const string& nombre, const string& apellido, const string& fecha_de_nacimiento, const string& password) {
        root = insertRec(root, email, nombre, apellido, fecha_de_nacimiento, password);
    }

    // Buscar usuario por correo
    bool search(const string& email) const {
        return searchRec(root, email) != nullptr;
    }

    // Recorrido inorder público
    void inorder() const {
        inorderRec(root);
        qDebug() << "";  // Añadir una línea en blanco al final
    }

    bool verifyCredentials(const string& email, const string& password) const {
        shared_ptr<Node> userNode = searchRec(root, email);  // Buscar el nodo por correo
        if (userNode && userNode->password == password) {    // Si el nodo existe y la contraseña coincide
            return true;
        }
        return false;
    }
};

#endif

