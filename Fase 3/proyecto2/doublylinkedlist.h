#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include <string>
#include <QDebug>
#include <sys/stat.h>  // Para la creación de directorios
#include <direct.h>
#include <QDir>
#include <fstream>
#include "arbolBinario.h"

using namespace std;

// Estructura para los nodos de comentarios
struct node_comment {
    int id;
    string correo;
    string comentario;
    string fecha;
    string hora;
    node_comment* next;

    node_comment(int id, string correo, string comentario, string fecha, string hora) {
        this->id = id;
        this->correo = correo;
        this->comentario = comentario;
        this->fecha = fecha;
        this->hora = hora;
        next = nullptr;
    }
};

// Estructura para los nodos de publicaciones
struct node_publi {
    int id;
    string correo;
    string contenido_correo;
    string fecha;
    string hora;
    string imagenPath;  // Nuevo campo para almacenar la ruta de la imagen
    node_publi* next;
    node_publi* prev;
    node_comment* commentsHead; // Cabeza de la lista de comentarios

    node_publi(int id, string correo, string contenido_correo, string fecha, string hora, string imagenPath = "")
        : id(id), correo(correo), contenido_correo(contenido_correo), fecha(fecha), hora(hora), imagenPath(imagenPath),
        next(nullptr), prev(nullptr), commentsHead(nullptr) {}
};

// Clase de la lista doblemente enlazada
class DoublyLinkedList {
private:
    node_publi* head;
    node_publi* tail;
    int nextId; // Para asignar IDs únicos a publicaciones
    int nextCommentId; // Para asignar IDs únicos a comentarios


    void crearCarpetaReportes() {
        const char* carpeta = "reportes";
        qDebug() << "Creando carpeta en ruta" << QDir::currentPath();
        if (_mkdir(carpeta) == -1) {
            if (errno == EEXIST) {
                qDebug() << "La carpeta 'reportes' ya existe.";
            } else {
                qDebug() << "Error al crear la carpeta 'reportes'.";
            }
        } else {
            qDebug() << "Carpeta 'reportes' creada con éxito.";
        }
    }

    void generateDotHelper(node_publi* node, std::ofstream& dotFile) {
        node_publi* current = node;
        while (current != nullptr) {
            // Asignar identificador único al nodo con los datos en filas utilizando \n para saltos de línea
            dotFile << "node" << current << " [label=\""
                    << "Correo: " << current->correo << "\\n"
                    << "Contenido: " << current->contenido_correo << "\\n"
                    << "Fecha: " << current->fecha << "\\n"
                    << "Hora: " << current->hora << "\" shape=box];\n";

            // Conexión con el siguiente nodo
            if (current->next != nullptr) {
                // Conexión de current a next
                dotFile << "node" << current << " -> node" << current->next << ";\n";
                // Conexión de next a current (doble enlace)
                dotFile << "node" << current->next << " -> node" << current << ";\n";
            }

            current = current->next;
        }
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), nextId(0), nextCommentId(0) {}


    int getNextId() const {
        return nextId;
    }

    // Método para añadir una publicación
    void append(string correo, string contenido_correo, string fecha, string hora, string imagenPath = "") {
        node_publi* newNode = new node_publi(nextId++, correo, contenido_correo, fecha, hora, imagenPath);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Método para añadir un comentario a una publicación
    void addComment(int idPublicacion, string correo, string comentario, string fecha, string hora) {
        node_publi* current = head;
        while (current != nullptr && current->id != idPublicacion) {
            current = current->next;
        }

        if (current != nullptr) {
            node_comment* newComment = new node_comment(nextCommentId++, correo, comentario, fecha, hora);
            if (current->commentsHead == nullptr) {
                current->commentsHead = newComment;
            } else {
                node_comment* temp = current->commentsHead;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newComment;
            }
        } else {
            cout << "No se encontró la publicación con ID: " << idPublicacion << endl;
        }
    }

    // Método para imprimir publicaciones y sus comentarios
    void print() {
        node_publi* current = head;
        while (current != nullptr) {
            cout << "ID: " << current->id << " | Correo: " << current->correo
                 << " | Contenido: " << current->contenido_correo
                 << " | Fecha: " << current->fecha << " | Hora: " << current->hora;

            if (!current->imagenPath.empty()) {
                cout << " | Imagen: " << current->imagenPath;  // Mostrar la ruta de la imagen si existe
            }
            cout << endl;

            // Imprimir comentarios asociados a la publicación
            node_comment* commentCurrent = current->commentsHead;
            while (commentCurrent != nullptr) {
                cout << "  Comentario de: " << commentCurrent->correo << " | Comentario: " << commentCurrent->comentario
                     << " | Fecha: " << commentCurrent->fecha << " | Hora: " << commentCurrent->hora << endl;
                commentCurrent = commentCurrent->next;
            }

            current = current->next;
        }
    }

    void generateDot() {
        // Crear la carpeta "reportes" si no existe
        crearCarpetaReportes();

        // Abrir el archivo DOT
        ofstream dotFile("reportes/listaPublicaciones.dot");
        dotFile << "digraph DoublyLinkedList {\n";
        dotFile << "rankdir=LR;\n"; // Orientación de izquierda a derecha
        dotFile << "node [shape=record];\n";

        // Llamar a la función helper para escribir los nodos y las conexiones
        generateDotHelper(head, dotFile);

        dotFile << "}\n";
        dotFile.close();

        // Crear el archivo PNG usando Graphviz
        string command = "dot -Tpng reportes/listaPublicaciones.dot -o reportes/listaPublicaciones.png";
        system(command.c_str());  // Ejecuta el comando para convertir DOT a PNG
    }



    // Destructor para liberar la memoria
    ~DoublyLinkedList() {
        node_publi* current = head;
        while (current != nullptr) {
            node_publi* next = current->next;
            node_comment* commentCurrent = current->commentsHead;

            // Liberar los comentarios asociados
            while (commentCurrent != nullptr) {
                node_comment* commentNext = commentCurrent->next;
                delete commentCurrent;
                commentCurrent = commentNext;
            }

            delete current;
            current = next;
        }
    }

    void agregarPublicacionesAlArbol(const string& userCorreo, ArbolBinario& arbol) {
        node_publi* current = head;

        // Recorrer la lista
        while (current != nullptr) {
            // Verificar si la publicación pertenece al usuario logueado
            if (current->correo == userCorreo) {
                // Si pertenece al usuario logueado, agregar la publicación al árbol binario
                arbol.insertarPublicacion(
                    current->fecha,       // Fecha de la publicación
                    current->correo,      // Correo del usuario (ya verificado)
                    current->contenido_correo, // Contenido de la publicación
                    current->hora,        // Hora de la publicación
                    current->imagenPath   // Ruta de la imagen (si existe)
                    );
            }
            current = current->next; // Avanzar al siguiente nodo
        }
    }
};

#endif // DOUBLYLINKEDLIST_H
