#ifndef AVL_H
#define AVL_H

#include <memory>
#include <string>
#include <QDebug>
#include <functional>

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

    // Funciones helper para los recorridos
    void inorderHelper(std::shared_ptr<Node> node, std::function<void(std::shared_ptr<Node>)> visit) {
        if (!node) return;
        inorderHelper(node->left, visit);
        visit(node);
        inorderHelper(node->right, visit);
    }

    void preorderHelper(std::shared_ptr<Node> node, std::function<void(std::shared_ptr<Node>)> visit) {
        if (!node) return;
        visit(node);
        preorderHelper(node->left, visit);
        preorderHelper(node->right, visit);
    }

    void postorderHelper(std::shared_ptr<Node> node, std::function<void(std::shared_ptr<Node>)> visit) {
        if (!node) return;
        postorderHelper(node->left, visit);
        postorderHelper(node->right, visit);
        visit(node);
    }


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

    // Eliminar un nodo basado en el correo electrónico
    shared_ptr<Node> deleteNodeRec(shared_ptr<Node> root, const string& email) {
        if (!root) {
            return root;  // Caso base: nodo no encontrado
        }

        if (email < root->email) {
            root->left = deleteNodeRec(root->left, email);
        } else if (email > root->email) {
            root->right = deleteNodeRec(root->right, email);
        } else {
            // Nodo encontrado, eliminarlo
            if (!root->left || !root->right) {
                // Nodo con un solo hijo o sin hijos
                shared_ptr<Node> temp = root->left ? root->left : root->right;

                // No tiene hijos
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {  // Un solo hijo
                    *root = *temp;  // Copiar el contenido del hijo no vacío
                }
            } else {
                // Nodo con dos hijos: obtener el sucesor inorder (el más pequeño en el subárbol derecho)
                shared_ptr<Node> temp = root->right;
                while (temp->left) {
                    temp = temp->left;
                }

                // Copiar los datos del sucesor
                root->email = temp->email;
                root->nombre = temp->nombre;
                root->apellido = temp->apellido;
                root->fecha_de_nacimiento = temp->fecha_de_nacimiento;
                root->password = temp->password;

                // Eliminar el sucesor inorder
                root->right = deleteNodeRec(root->right, temp->email);
            }
        }

        if (!root) {
            return root;
        }

        // Actualizar la altura
        root->height = 1 + max(height(root->left), height(root->right));

        // Obtener el factor de balance
        int balance = getBalance(root);

        // Balancear el árbol si es necesario
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }


public:
    AVL() : root(nullptr) {}

    // Método para obtener la raíz del árbol
    shared_ptr<Node> getRoot() const {
        return root;
    }


    // Inserción pública
    void insert(const string& email, const string& nombre, const string& apellido, const string& fecha_de_nacimiento, const string& password) {
        root = insertRec(root, email, nombre, apellido, fecha_de_nacimiento, password);
    }

    // Buscar usuario por correo
    bool search(const string& email) const {
        return searchRec(root, email) != nullptr;
    }

    shared_ptr<Node> getNode(const string& email) const {
        return searchRec(root, email);  // Llama al método privado searchRec
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

    void deleteNode(const string& email) {
        root = deleteNodeRec(root, email);
    }

    // Métodos de recorrido público
    void inorderTraversal(std::function<void(std::shared_ptr<Node>)> visit) {
        inorderHelper(root, visit);
    }

    void preorderTraversal(std::function<void(std::shared_ptr<Node>)> visit) {
        preorderHelper(root, visit);
    }

    void postorderTraversal(std::function<void(std::shared_ptr<Node>)> visit) {
        postorderHelper(root, visit);
    }
};

#endif

