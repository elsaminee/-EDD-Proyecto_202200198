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

class NodoSugerencia {
public:
    string email;   // Correo del amigo sugerido
    int amigosEnComun;  // Número de amigos en común
    NodoSugerencia* siguiente;

    NodoSugerencia(const string& email, int amigosEnComun)
        : email(email), amigosEnComun(amigosEnComun), siguiente(nullptr) {}
};

// Lista enlazada para almacenar las sugerencias
class ListaSugerencia {
public:
    NodoSugerencia* head;

    ListaSugerencia() : head(nullptr) {}

    // Insertar un nodo de sugerencia de manera ordenada por cantidad de amigos en común
    void insertarOrdenado(const string& email, int amigosEnComun) {
        NodoSugerencia* nuevo = new NodoSugerencia(email, amigosEnComun);
        if (!head || amigosEnComun > head->amigosEnComun) {
            nuevo->siguiente = head;
            head = nuevo;
        } else {
            NodoSugerencia* actual = head;
            while (actual->siguiente && amigosEnComun <= actual->siguiente->amigosEnComun) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
    }

    // Mostrar las sugerencias
    void mostrar() {
        NodoSugerencia* actual = head;
        while (actual) {
            cout << "Correo: " << actual->email << " | Amigos en común: " << actual->amigosEnComun << endl;
            actual = actual->siguiente;
        }
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

    shared_ptr<NodoAdyacencia> head = nullptr;

    // Método para sugerir amigos basado en amigos de amigos (distancia de dos saltos)
    ListaSugerencia sugerirAmigos(const string& correoUsuario) {
        ListaSugerencia sugerencias;  // Lista para almacenar las sugerencias
        shared_ptr<NodoAdyacencia> usuario = buscarNodo(correoUsuario);
        if (!usuario) {
            cout << "Usuario no encontrado." << endl;
            return sugerencias;
        }

        // Lista enlazada de amigos directos del usuario
        shared_ptr<SubNodoAdyacencia> amigosUsuario = usuario->listaAmigos;

        // Paso 1: Recorrer los amigos directos del usuario
        while (amigosUsuario) {
            shared_ptr<NodoAdyacencia> amigo = buscarNodo(amigosUsuario->correoAmigo);
            if (amigo) {
                // Paso 2: Recorrer los amigos de los amigos
                shared_ptr<SubNodoAdyacencia> amigosDeAmigo = amigo->listaAmigos;
                while (amigosDeAmigo) {
                    // Evitar sugerir al propio usuario o a sus amigos directos
                    if (amigosDeAmigo->correoAmigo != correoUsuario && !esAmigoDirecto(correoUsuario, amigosDeAmigo->correoAmigo)) {
                        // Incrementar el contador de amigos en común
                        NodoSugerencia* sugerenciaExistente = buscarSugerencia(sugerencias, amigosDeAmigo->correoAmigo);
                        if (sugerenciaExistente) {
                            sugerenciaExistente->amigosEnComun++;
                        } else {
                            sugerencias.insertarOrdenado(amigosDeAmigo->correoAmigo, 1);
                        }
                    }
                    amigosDeAmigo = amigosDeAmigo->siguiente;
                }
            }
            amigosUsuario = amigosUsuario->siguiente;
        }

        return sugerencias;  // Retornar las sugerencias ordenadas
    }

    // Método auxiliar para verificar si dos usuarios son amigos directos
    bool esAmigoDirecto(const string& correoUsuario, const string& correoAmigo) {
        shared_ptr<NodoAdyacencia> usuario = buscarNodo(correoUsuario);
        if (!usuario) return false;

        shared_ptr<SubNodoAdyacencia> amigosUsuario = usuario->listaAmigos;
        while (amigosUsuario) {
            if (amigosUsuario->correoAmigo == correoAmigo) {
                return true;  // Son amigos directos
            }
            amigosUsuario = amigosUsuario->siguiente;
        }
        return false;  // No son amigos directos
    }

    // Buscar sugerencia existente
    NodoSugerencia* buscarSugerencia(ListaSugerencia& lista, const string& correoAmigo) {
        NodoSugerencia* actual = lista.head;
        while (actual) {
            if (actual->email == correoAmigo) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

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

    void graph_listaEnlazada() {
        ofstream file("graph_lista_enlazada.dot");

        // Iniciar la definición del grafo con rankdir=LR para un flujo de izquierda a derecha
        file << "digraph G {" << endl;
        file << "rankdir=LR;" << endl;  // Definir el flujo de izquierda a derecha
        file << "node [shape=record];" << endl;  // Usar nodos con forma de registros

        shared_ptr<NodoAdyacencia> temp = cabeza;

        // Para asegurar nombres únicos, vamos a usar un contador global
        int contador = 1;

        while (temp) {
            // Definir el nodo principal (usuario)
            string nodoPrincipal = "\"" + temp->correoUsuario + "\"";
            file << nodoPrincipal << " [label=\"" << temp->correoUsuario << "\"];" << endl;

            // Añadir amigos como nodos independientes
            shared_ptr<SubNodoAdyacencia> tempAmigo = temp->listaAmigos;
            string nodoAmigoPrevio;  // Para almacenar el nodo amigo anterior

            while (tempAmigo) {
                // Cada amigo tiene un nodo único basado en el usuario y el amigo
                string nodoAmigo = "\"" + tempAmigo->correoAmigo + "_" + to_string(contador) + "\"";
                file << nodoAmigo << " [label=\"" << tempAmigo->correoAmigo << "\"];" << endl;

                // Si hay un nodo amigo previo, conectarlo con el nuevo nodo amigo
                if (!nodoAmigoPrevio.empty()) {
                    file << nodoAmigoPrevio << " -> " << nodoAmigo << ";" << endl;
                } else {
                    // Si es el primer amigo, conectarlo con el nodo principal
                    file << nodoPrincipal << " -> " << nodoAmigo << ";" << endl;
                }

                // Actualizar el nodo amigo previo al actual
                nodoAmigoPrevio = nodoAmigo;

                tempAmigo = tempAmigo->siguiente;
                contador++;
            }

            // Incrementar el contador global después de procesar cada usuario
            temp = temp->siguiente;  // Pasar al siguiente usuario
        }

        file << "}" << endl;  // Cerrar el archivo .dot
        file.close();

        // Convertir el archivo .dot en una imagen .png
        string command = "dot -Tpng graph_lista_enlazada.dot -o graph_lista_enlazada.png";
        if (system(command.c_str()) == 0) {
            cout << "Lista enlazada creada exitosamente" << endl;
        } else {
            cout << "Error al crear la lista enlazada" << endl;
        }
    }
};
#endif // GRAFONDIRIGIDO_H

