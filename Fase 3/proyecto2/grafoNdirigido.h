#ifndef GRAFONDIRIGIDO_H
#define GRAFONDIRIGIDO_H

#include <iostream>
#include <memory>
#include <fstream>
#include <string>

using namespace std;

class SubNodoAdyacencia {
public:
    string correoAmigo;   // Correo del amigo
    shared_ptr<SubNodoAdyacencia> siguiente = nullptr;

    SubNodoAdyacencia(const string& correo) : correoAmigo(correo) {}
};

class NodoAdyacencia {
public:
    string correoUsuario;  // Correo del usuario principal
    shared_ptr<NodoAdyacencia> siguiente = nullptr;
    shared_ptr<NodoAdyacencia> anterior = nullptr;
    shared_ptr<SubNodoAdyacencia> listaAmigos = nullptr;

    NodoAdyacencia(const string& correo) : correoUsuario(correo) {}

    // Método para agregar un amigo a la lista de amigos
    void agregarAmigo(const string& correoAmigo) {
        shared_ptr<SubNodoAdyacencia> nuevoAmigo = make_shared<SubNodoAdyacencia>(correoAmigo);

        if (!listaAmigos) {
            listaAmigos = nuevoAmigo;
        } else {
            shared_ptr<SubNodoAdyacencia> temp = listaAmigos;
            while (temp->siguiente) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoAmigo;
        }
    }

    // Método para verificar si ya existe una relación de amistad
    bool existeAmigo(const string& correoAmigo) {
        shared_ptr<SubNodoAdyacencia> temp = listaAmigos;
        while (temp) {
            if (temp->correoAmigo == correoAmigo) {
                return true;  // Si el amigo ya existe
            }
            temp = temp->siguiente;
        }
        return false;  // No se encontró el amigo
    }

    // Método para imprimir los amigos del usuario
    void imprimirAmigos() {
        shared_ptr<SubNodoAdyacencia> temp = listaAmigos;
        cout << correoUsuario << " -> ";
        while (temp) {
            cout << temp->correoAmigo << " ";
            temp = temp->siguiente;
        }
        cout << endl;
    }
};

class ListaAdyacencia {
private:
    shared_ptr<NodoAdyacencia> cabeza = nullptr;
    shared_ptr<NodoAdyacencia> cola = nullptr;

    // Método para buscar un nodo por correo
    shared_ptr<NodoAdyacencia> buscarNodo(const string& correoUsuario) {
        shared_ptr<NodoAdyacencia> temp = cabeza;
        while (temp) {
            if (temp->correoUsuario == correoUsuario) {
                return temp;  // Nodo encontrado
            }
            temp = temp->siguiente;
        }
        return nullptr;  // Nodo no encontrado
    }

public:
    // Método para agregar una relación de amistad
    void agregarRelacion(const string& correo1, const string& correo2) {
        shared_ptr<NodoAdyacencia> nodo1 = buscarNodo(correo1);
        shared_ptr<NodoAdyacencia> nodo2 = buscarNodo(correo2);

        // Verificar si la relación ya existe en cualquiera de las dos direcciones
        if ((nodo1 && nodo1->existeAmigo(correo2)) || (nodo2 && nodo2->existeAmigo(correo1))) {
            cout << "La relación de amistad ya existe entre " << correo1 << " y " << correo2 << endl;
            return;
        }

        // Si no existe el nodo para correo1, lo creamos
        if (!nodo1) {
            nodo1 = make_shared<NodoAdyacencia>(correo1);
            if (!cabeza) {
                cabeza = nodo1;
                cola = nodo1;
            } else {
                cola->siguiente = nodo1;
                nodo1->anterior = cola;
                cola = nodo1;
            }
        }
        // Si no existe el nodo para correo2, lo creamos
        if (!nodo2) {
            nodo2 = make_shared<NodoAdyacencia>(correo2);
            if (!cabeza) {
                cabeza = nodo2;
                cola = nodo2;
            } else {
                cola->siguiente = nodo2;
                nodo2->anterior = cola;
                cola = nodo2;
            }
        }

        // Agregar correo2 a la lista de amigos de correo1 y viceversa
        nodo1->agregarAmigo(correo2);
        nodo2->agregarAmigo(correo1);
    }

    // Método para imprimir todas las relaciones de amistad
    void imprimirRelaciones() {
        shared_ptr<NodoAdyacencia> temp = cabeza;
        while (temp) {
            temp->imprimirAmigos();
            temp = temp->siguiente;
        }
    }

    // Método para generar el grafo en formato dot y convertirlo en una imagen
    void graph() {
        ofstream file("graph.dot");

        file << "graph G {" << endl;  // Usamos graph en lugar de digraph para un grafo no dirigido

        shared_ptr<NodoAdyacencia> temp = cabeza;
        while (temp) {
            shared_ptr<SubNodoAdyacencia> tempAmigo = temp->listaAmigos;
            while (tempAmigo) {
                // Añadir la relación al archivo dot, sin dirección (simétrica)
                file << "\"" << temp->correoUsuario << "\" -- \"" << tempAmigo->correoAmigo << "\";" << endl;
                tempAmigo = tempAmigo->siguiente;
            }
            temp = temp->siguiente;
        }

        file << "}" << endl;
        file.close();

        // Convertir el archivo .dot en una imagen .png
        string command = "dot -Tpng graph.dot -o graph.png";
        if (system(command.c_str()) == 0) {
            cout << "Graph created successfully" << endl;
        } else {
            cout << "Error creating graph" << endl;
        }
    }
};
#endif // GRAFONDIRIGIDO_H
