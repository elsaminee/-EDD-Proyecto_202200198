#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <iomanip>  // Incluir para el uso de setw
#include <cstdlib>  // Para el uso de system()
#include <fstream>

using namespace std;
struct NodeVal {
public:
    bool exists = false;
    string value = "";
};

// Clase Node para cada nodo de la matriz
struct NodeMatrix {
public:
    string i = "";
    string j = "";
    string value = "";
    NodeMatrix* up = nullptr;
    NodeMatrix* down = nullptr;
    NodeMatrix* right = nullptr;
    NodeMatrix* left = nullptr;

    NodeMatrix(string i = "", string j = "", string value = "") : i(i), j(j), value(value) {}
};

// Clase Matrix que maneja la matriz
class Matrix {
private:
    NodeMatrix* root = nullptr;
    NodeMatrix* currentFriend = nullptr;

public:
    // Método para insertar un nuevo nodo en la matriz
    void insert(string i, string j, string value = "X") {
        NodeMatrix* newNode = new NodeMatrix(i, j, value);

        if (!root) {
            root = new NodeMatrix();
        }

        NodeMatrix* row = searchRow(i);
        NodeMatrix* column = searchColumn(j);

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
    NodeMatrix* searchRow(string i) {
        NodeMatrix* current = root->down;

        while (current) {
            if (current->i == i) return current;
            current = current->down;
        }
        return nullptr;
    }

    // Método para buscar una columna por su índice
    NodeMatrix* searchColumn(string j) {
        NodeMatrix* current = root->right;

        while (current) {
            if (current->j == j) return current;
            current = current->right;
        }
        return nullptr;
    }

    // Método para verificar si un nodo ya existe
    bool nodeExists(NodeMatrix* newNode) {
        NodeMatrix* rowHeader = searchRow(newNode->i);

        if (rowHeader) {
            NodeMatrix* column = rowHeader->right;
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
    NodeMatrix* insertRowHeader(string i) {
        NodeMatrix* newRowHeader = new NodeMatrix(i, "");
        insertInColumn(newRowHeader, root);
        return newRowHeader;
    }

    // Método para insertar un encabezado de columna
    NodeMatrix* insertColumnHeader(string j) {
        NodeMatrix* newColumnHeader = new NodeMatrix("", j);
        insertInRow(newColumnHeader, root);
        return newColumnHeader;
    }

    // Método para insertar un nodo en una fila
    void insertInRow(NodeMatrix* newNode, NodeMatrix* rowHeader) {
        NodeMatrix* current = rowHeader;

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
    void insertInColumn(NodeMatrix* newNode, NodeMatrix* columnHeader) {
        NodeMatrix* current = columnHeader;

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

        NodeMatrix* rowHeader = root->down;
        while (rowHeader) {
            cout << "\n" << setw(8) << rowHeader->i;
            NodeMatrix* column = rowHeader->right;
            for (NodeMatrix* colHeader = root->right; colHeader; colHeader = colHeader->right) {
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
        for (NodeMatrix* colHeader = root->right; colHeader; colHeader = colHeader->right) {
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
        NodeMatrix* columnHeader = root->right;
        while (columnHeader) {
            dotFile << "<TD>" << columnHeader->j << "</TD>";
            columnHeader = columnHeader->right;
        }
        dotFile << "</TR>" << endl;

        // Filas de la matriz
        NodeMatrix* rowHeader = root->down;
        while (rowHeader) {
            dotFile << "    <TR><TD>" << rowHeader->i << "</TD>";
            columnHeader = root->right;
            while (columnHeader) {
                NodeMatrix* current = rowHeader->right;
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
        NodeMatrix* rowHeader = searchRow(person);
        if (rowHeader) {
            currentFriend = rowHeader->right;
        } else {
            currentFriend = nullptr;
        }
        NodeMatrix* columnHeader = searchColumn(person);
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

        NodeMatrix* rowHeader = root->down;
        while (rowHeader && userCount < maxUsers) {
            // Almacenar el nombre del usuario
            users[userCount] = rowHeader->i;

            // Contar cuántos amigos tiene el usuario
            NodeMatrix* current = rowHeader->right;
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

        NodeMatrix* rowHeader = root->down;
        while (rowHeader && userCount < maxUsers) {
            // Almacenar el nombre del usuario
            users[userCount] = rowHeader->i;

            // Contar cuántas publicaciones tiene el usuario
            NodeMatrix* current = rowHeader->right;
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
        NodeMatrix* rowHeader = searchRow(correo);
        if (rowHeader) {
            // Eliminar el nodo de la fila
            NodeMatrix* current = rowHeader->right;
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
        NodeMatrix* columnHeader = searchColumn(correo);
        if (columnHeader) {
            // Eliminar el nodo de la columna
            NodeMatrix* current = columnHeader->down;
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
            NodeMatrix* prevRowHeader = root;
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
            NodeMatrix* prevColumnHeader = root;
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
