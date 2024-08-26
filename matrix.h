#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

using namespace std;
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

    void generateDot(const string& filename) const {
        ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            cerr << "No se pudo crear el archivo DOT." << endl;
            return;
        }

        dotFile << "digraph G {" << endl;
        dotFile << "    node [shape=none];" << endl;
        dotFile << "    graph [rankdir=LR];" << endl;  // Para mantener la forma de matriz
        
        dotFile << "    matriz [label=<" << endl;
        dotFile << "    <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">" << endl;

        // Encabezados de columnas
        dotFile << "    <TR><TD></TD>";
        Node* columnHeader = root->right;
        while (columnHeader) {
            dotFile << "<TD>" << columnHeader->j << "</TD>";
            columnHeader = columnHeader->right;
        }
        dotFile << "</TR>" << endl;

        // Filas de la matriz
        Node* rowHeader = root->down;
        while (rowHeader) {
            dotFile << "    <TR><TD>" << rowHeader->i << "</TD>";
            columnHeader = root->right;
            while (columnHeader) {
                Node* current = rowHeader->right;
                bool found = false;
                while (current) {
                    if (current->j == columnHeader->j) {
                        dotFile << "<TD>X</TD>";
                        found = true;
                        break;
                    }
                    current = current->right;
                }
                if (!found) {
                    dotFile << "<TD></TD>";
                }
                columnHeader = columnHeader->right;
            }
            dotFile << "</TR>" << endl;
            rowHeader = rowHeader->down;
        }

        dotFile << "    </TABLE>>];" << endl;
        dotFile << "}" << endl;
        dotFile.close();

        // Generar el archivo PNG desde el archivo DOT
        string command = "dot -Tpng " + filename + " -o amistad.png";
        system(command.c_str());
    }

    void startFriendIteration(string person) {
        Node* rowHeader = searchRow(person);
        if (rowHeader) {
            currentFriend = rowHeader->right;
        } else {
            currentFriend = nullptr;
        }
        Node* columnHeader = searchColumn(person);
        if (columnHeader) {
            currentFriend = columnHeader->down;
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

    void printTopUsersWithLeastFriends(int topN) const {
        const int maxUsers = 100;
        string users[maxUsers];
        int friendCounts[maxUsers] = {0}; // Inicializa todos los conteos a 0
        int userCount = 0;

        Node* rowHeader = root->down;
        while (rowHeader && userCount < maxUsers) {
            // Almacenar el nombre del usuario
            users[userCount] = rowHeader->i;

            // Contar cuántos amigos tiene el usuario
            Node* current = rowHeader->right;
            while (current) {
                friendCounts[userCount]++;
                current = current->right;
            }

            userCount++;
            rowHeader = rowHeader->down;
        }

        // Ordenar usuarios por número de amigos en orden ascendente
        for (int i = 0; i < userCount - 1; i++) {
            for (int j = i + 1; j < userCount; j++) {
                if (friendCounts[i] > friendCounts[j]) {
                    // Intercambiar los conteos
                    int tempCount = friendCounts[i];
                    friendCounts[i] = friendCounts[j];
                    friendCounts[j] = tempCount;

                    // Intercambiar los nombres de usuarios
                    string tempUser = users[i];
                    users[i] = users[j];
                    users[j] = tempUser;
                }
            }
        }

        // Imprimir el top N usuarios con menos amigos, incluyendo aquellos con 0 amigos
        cout << "Top " << topN << " usuarios con menos amigos:" << endl;
        for (int i = 0; i < topN && i < userCount; i++) {
            cout << "Usuario: " << users[i] << " | Número de amigos: " << friendCounts[i] << endl;
        }
    }

    void printTopUsersWithMostPosts(int topN) const {
        const int maxUsers = 100;
        string users[maxUsers];
        int postCounts[maxUsers] = {0}; // Inicializa todos los conteos a 0
        int userCount = 0;

        Node* rowHeader = root->down;
        while (rowHeader && userCount < maxUsers) {
            // Almacenar el nombre del usuario
            users[userCount] = rowHeader->i;

            // Contar cuántas publicaciones tiene el usuario
            Node* current = rowHeader->right;
            while (current) {
                postCounts[userCount]++;
                current = current->right;
            }

            userCount++;
            rowHeader = rowHeader->down;
        }

        // Ordenar usuarios por número de publicaciones en orden descendente usando un método simple
        for (int i = 0; i < userCount - 1; i++) {
            for (int j = i + 1; j < userCount; j++) {
                if (postCounts[i] < postCounts[j]) {
                    // Intercambiar los conteos
                    int tempCount = postCounts[i];
                    postCounts[i] = postCounts[j];
                    postCounts[j] = tempCount;

                    // Intercambiar los nombres de usuarios
                    string tempUser = users[i];
                    users[i] = users[j];
                    users[j] = tempUser;
                }
            }
        }

        // Imprimir el top N usuarios con más publicaciones
        cout << "Top " << topN << " usuarios con más publicaciones:" << endl;
        for (int i = 0; i < topN && i < userCount; i++) {
            cout << "Usuario: " << users[i] << " | Número de publicaciones: " << postCounts[i] << endl;
        }
    }

void deleteNode(const string& correo) {
    // Buscar el nodo en la fila correspondiente
    Node* rowHeader = searchRow(correo);
    if (rowHeader) {
        // Eliminar el nodo de la fila
        Node* current = rowHeader->right;
        while (current) {
            if (current->j == correo) {
                // Ajustar las conexiones en la fila
                if (current->left) {
                    current->left->right = current->right;
                }
                if (current->right) {
                    current->right->left = current->left;
                }
                delete current;
                break;
            }
            current = current->right;
        }
    }

    // Buscar el nodo en la columna correspondiente
    Node* columnHeader = searchColumn(correo);
    if (columnHeader) {
        // Eliminar el nodo de la columna
        Node* current = columnHeader->down;
        while (current) {
            if (current->i == correo) {
                // Ajustar las conexiones en la columna
                if (current->up) {
                    current->up->down = current->down;
                }
                if (current->down) {
                    current->down->up = current->up;
                }
                delete current;
                break;
            }
            current = current->down;
        }
    }

    // Si el encabezado de la fila está vacío, eliminar el encabezado de la fila
    if (rowHeader && !rowHeader->right) {
        // Eliminar el encabezado de la fila
        Node* prevRowHeader = root;
        while (prevRowHeader && prevRowHeader->down != rowHeader) {
            prevRowHeader = prevRowHeader->down;
        }
        if (prevRowHeader) {
            prevRowHeader->down = rowHeader->down;
            if (rowHeader->down) {
                rowHeader->down->up = prevRowHeader;
            }
            delete rowHeader;
        }
    }

    // Si el encabezado de la columna está vacío, eliminar el encabezado de la columna
    if (columnHeader && !columnHeader->down) {
        // Eliminar el encabezado de la columna
        Node* prevColumnHeader = root;
        while (prevColumnHeader && prevColumnHeader->right != columnHeader) {
            prevColumnHeader = prevColumnHeader->right;
        }
        if (prevColumnHeader) {
            prevColumnHeader->right = columnHeader->right;
            if (columnHeader->right) {
                columnHeader->right->left = prevColumnHeader;
            }
            delete columnHeader;
        }
    }
}

};

#endif