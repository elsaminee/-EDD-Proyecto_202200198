#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// Clase NodeVal para manejar los valores en la matriz
class NodeVal {
public:
    bool exists = false;
    string value = "";
};

// Clase Node para cada nodo de la matriz
class Node {
public:
    string i = "";
    string j = "";
    string value = "";
    Node* up = nullptr;
    Node* down = nullptr;
    Node* right = nullptr;
    Node* left = nullptr;

    Node(string i = "", string j = "", string value = "") : i(i), j(j), value(value) {}
};

// Clase Matrix que maneja la matriz
class Matrix {
private:
    Node* root = nullptr;
    Node* currentFriend = nullptr;

public:
    // Método para insertar un nuevo nodo en la matriz
    void insert(string i, string j, string value = "X") {
        Node* newNode = new Node(i, j, value);

        if (!root) {
            root = new Node();
        }

        Node* row = searchRow(i);
        Node* column = searchColumn(j);

        if (!nodeExists(newNode)) {
            if (!row) {
                row = insertRowHeader(i);
            }

            if (!column) {
                column = insertColumnHeader(j);
            }

            insertInRow(newNode, row);
            insertInColumn(newNode, column);
        }
    }

    // Método para buscar una fila por su índice
    Node* searchRow(string i) {
        Node* current = root->down;

        while (current) {
            if (current->i == i) return current;
            current = current->down;
        }
        return nullptr;
    }

    // Método para buscar una columna por su índice
    Node* searchColumn(string j) {
        Node* current = root->right;

        while (current) {
            if (current->j == j) return current;
            current = current->right;
        }
        return nullptr;
    }

    // Método para verificar si un nodo ya existe
    bool nodeExists(Node* newNode) {
        Node* rowHeader = searchRow(newNode->i);

        if (rowHeader) {
            Node* column = rowHeader->right;
            while (column) {
                if (column->j == newNode->j) {
                    column->value = newNode->value;
                    return true;
                }
                column = column->right;
            }
        }
        return false;
    }

    // Método para insertar un encabezado de fila
    Node* insertRowHeader(string i) {
        Node* newRowHeader = new Node(i, "");
        insertInColumn(newRowHeader, root);
        return newRowHeader;
    }

    // Método para insertar un encabezado de columna
    Node* insertColumnHeader(string j) {
        Node* newColumnHeader = new Node("", j);
        insertInRow(newColumnHeader, root);
        return newColumnHeader;
    }

    // Método para insertar un nodo en una fila
    void insertInRow(Node* newNode, Node* rowHeader) {
        Node* current = rowHeader;

        while (current->right) {
            if (newNode->j < current->right->j) {
                newNode->right = current->right;
                newNode->left = current;
                current->right->left = newNode;
                current->right = newNode;
                return;
            }
            current = current->right;
        }

        current->right = newNode;
        newNode->left = current;
    }

    // Método para insertar un nodo en una columna
    void insertInColumn(Node* newNode, Node* columnHeader) {
        Node* current = columnHeader;

        while (current->down) {
            if (newNode->i < current->down->i) {
                newNode->down = current->down;
                newNode->up = current;
                current->down->up = newNode;
                current->down = newNode;
                return;
            }
            current = current->down;
        }

        current->down = newNode;
        newNode->up = current;
    }

    // Método para imprimir la matriz
    void print() const {
        printColumnHeaders();

        Node* rowHeader = root->down;
        while (rowHeader) {
            cout << "\n" << setw(8) << rowHeader->i;
            Node* column = rowHeader->right;
            for (Node* colHeader = root->right; colHeader; colHeader = colHeader->right) {
                if (column && column->j == colHeader->j) {
                    cout << setw(8) << column->value;
                    column = column->right;
                } else {
                    cout << setw(8) << " ";
                }
            }
            rowHeader = rowHeader->down;
        }
        cout << endl;
    }

    // Método para imprimir los encabezados de las columnas
    void printColumnHeaders() const {
        cout << setw(8) << " ";
        for (Node* colHeader = root->right; colHeader; colHeader = colHeader->right) {
            cout << setw(8) << colHeader->j;
        }
        cout << endl;
    }

    void startFriendIteration(string person) {
        Node* rowHeader = searchRow(person);
        if (rowHeader) {
            currentFriend = rowHeader->right;
        } else {
            currentFriend = nullptr;
        }
    }

    // Método para obtener el siguiente amigo
    string getNextFriend() {
        if (currentFriend) {
            string friendName = currentFriend->j;
            currentFriend = currentFriend->right;
            return friendName;
        }
        return "";  // Retorna cadena vacía si no hay más amigos
    }
};