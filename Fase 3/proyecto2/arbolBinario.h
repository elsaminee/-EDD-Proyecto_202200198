#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include <iostream>
#include <string>
#include <memory>
#include <functional>  // Para usar std::function

using namespace std;


struct NodoComentario {
    string correo;  // Correo del autor del comentario
    string comentario;

    NodoComentario* siguiente;

    NodoComentario(const string& correo, const string& comentario)
        : correo(correo), comentario(comentario), siguiente(nullptr) {}
};

// Estructura para la lista simplemente enlazada que almacena publicaciones
struct NodoPublicacion {
    string correo;     // Nuevo atributo para el correo
    string contenido;
    string hora;       // Variable para la hora de la publicación
    string imagen;     // Campo para la imagen
    NodoPublicacion* siguiente;

    // Lista enlazada de comentarios
    NodoComentario* commentsHead;

    NodoPublicacion(const string& correo, const string& contenido, const string& hora, const string& imagen)
        : correo(correo), contenido(contenido), hora(hora), imagen(imagen), siguiente(nullptr), commentsHead(nullptr) {}
};



// Estructura del nodo del árbol binario de búsqueda
struct NodoArbol {
    string fecha;
    NodoPublicacion* listaPublicaciones;
    shared_ptr<NodoArbol> izquierdo;
    shared_ptr<NodoArbol> derecho;

    NodoArbol(const string& fecha) : fecha(fecha), listaPublicaciones(nullptr), izquierdo(nullptr), derecho(nullptr) {}
};

// Clase para manejar el árbol binario de búsqueda de publicaciones
class ArbolBinario {
private:
    shared_ptr<NodoArbol> raiz;

    // Función auxiliar para insertar en el árbol binario
    shared_ptr<NodoArbol> insertar(shared_ptr<NodoArbol> nodo, const string& fecha, const string& correo, const string& contenido, const string& hora, const string& imagen) {
        if (!nodo) {
            nodo = make_shared<NodoArbol>(fecha);
            nodo->listaPublicaciones = new NodoPublicacion(correo, contenido, hora, imagen);
            return nodo;
        }

        if (fecha < nodo->fecha) {
            nodo->izquierdo = insertar(nodo->izquierdo, fecha, correo, contenido, hora, imagen);
        } else if (fecha > nodo->fecha) {
            nodo->derecho = insertar(nodo->derecho, fecha, correo, contenido, hora, imagen);
        } else {
            // Si la fecha ya existe, agregar la publicación a la lista de publicaciones
            NodoPublicacion* nuevaPublicacion = new NodoPublicacion(correo, contenido, hora, imagen);
            nuevaPublicacion->siguiente = nodo->listaPublicaciones;
            nodo->listaPublicaciones = nuevaPublicacion;
        }

        return nodo;
    }

    // Función auxiliar para mostrar las publicaciones por fecha (InOrder traversal)
    void mostrarInOrder(shared_ptr<NodoArbol> nodo) const {
        if (!nodo) return;

        mostrarInOrder(nodo->izquierdo);
        cout << "Fecha: " << nodo->fecha << endl;
        NodoPublicacion* actual = nodo->listaPublicaciones;
        while (actual) {
            cout << "  Correo: " << actual->correo << " - Hora: " << actual->hora << " - Publicación: " << actual->contenido;
            if (!actual->imagen.empty()) {
                cout << " (Imagen: " << actual->imagen << ")";
            }
            cout << endl;
            actual = actual->siguiente;
        }
        mostrarInOrder(nodo->derecho);
    }

    // Función auxiliar para buscar publicaciones por fecha
    shared_ptr<NodoArbol> buscarFecha(shared_ptr<NodoArbol> nodo, const string& fecha) const {
        if (!nodo || nodo->fecha == fecha) return nodo;

        if (fecha < nodo->fecha) {
            return buscarFecha(nodo->izquierdo, fecha);
        } else {
            return buscarFecha(nodo->derecho, fecha);
        }
    }

    void eliminarPublicaciones(NodoPublicacion* publicacion) {
        while (publicacion) {
            NodoPublicacion* temp = publicacion;
            publicacion = publicacion->siguiente;
            delete temp;  // Liberar la memoria de cada publicación
        }
    }

    // Función auxiliar para eliminar los nodos en postorder
    void eliminarNodos(shared_ptr<NodoArbol>& nodo) {
        if (!nodo) return;

        // Primero eliminamos los nodos hijos
        eliminarNodos(nodo->izquierdo);
        eliminarNodos(nodo->derecho);

        // Luego eliminamos las publicaciones asociadas a este nodo
        eliminarPublicaciones(nodo->listaPublicaciones);

        // Finalmente, liberamos el nodo (esto lo maneja el `shared_ptr`)
        nodo.reset();  // Reseteamos el shared_ptr para liberar el nodo
    }

    // Función auxiliar para insertar un comentario en una publicación específica
    void insertarComentarioEnPublicacion(NodoPublicacion* publicacion, const string& correo, const string& comentario) {
        // Crear un nuevo comentario
        NodoComentario* nuevoComentario = new NodoComentario(correo, comentario);

        // Insertar el nuevo comentario al principio de la lista de comentarios de la publicación correcta
        nuevoComentario->siguiente = publicacion->commentsHead;
        publicacion->commentsHead = nuevoComentario;
    }

    // Función auxiliar para insertar comentarios en el árbol
    shared_ptr<NodoArbol> insertarComentarioEnArbol(shared_ptr<NodoArbol> nodo, const string& fecha, const string& correo, const string& comentario) {
        if (!nodo) {
            return nodo; // No se encuentra el nodo para esta fecha
        }

        if (fecha < nodo->fecha) {
            nodo->izquierdo = insertarComentarioEnArbol(nodo->izquierdo, fecha, correo, comentario);
        } else if (fecha > nodo->fecha) {
            nodo->derecho = insertarComentarioEnArbol(nodo->derecho, fecha, correo, comentario);
        } else {
            // Si la fecha coincide, insertamos el comentario en la lista de publicaciones
            NodoPublicacion* publicacion = nodo->listaPublicaciones;
            insertarComentarioEnPublicacion(publicacion, correo, comentario);
        }

        return nodo;
    }

public:
    // Constructor sin parámetros específicos
    ArbolBinario() : raiz(nullptr) {}

    // Mostrar todas las publicaciones en orden cronológico
    void mostrarPublicaciones() const {
        mostrarInOrder(raiz);
    }

    // Función para insertar un comentario asociado a una publicación específica
    void insertarComentario(const string& fechaPublicacion, const string& correoComentario, const string& comentario) {
        raiz = insertarComentarioEnArbol(raiz, fechaPublicacion, correoComentario, comentario);
    }

    // Insertar una nueva publicación con su fecha, correo, hora e imagen
    void insertarPublicacion(const string& fecha, const string& correo, const string& contenido, const string& hora, const string& imagen) {
        raiz = insertar(raiz, fecha, correo, contenido, hora, imagen);
    }

    // Buscar y mostrar publicaciones de una fecha específica
    void buscarPublicacionesPorFecha(const string& fecha) const {
        shared_ptr<NodoArbol> nodo = buscarFecha(raiz, fecha);
        if (nodo) {
            cout << "Publicaciones para la fecha " << fecha << ":" << endl;
            NodoPublicacion* actual = nodo->listaPublicaciones;
            while (actual) {
                cout << "  Correo: " << actual->correo << " - Hora: " << actual->hora << " - Publicación: " << actual->contenido;
                if (!actual->imagen.empty()) {
                    cout << " (Imagen: " << actual->imagen << ")";
                }
                cout << endl;
                actual = actual->siguiente;
            }
        } else {
            cout << "No se encontraron publicaciones para la fecha " << fecha << endl;
        }
    }

    // Recorrer las publicaciones con un callback
    void recorrerPublicaciones(function<void(const string&, NodoPublicacion*)> callback) const {
        recorrerInOrder(raiz, callback);
    }

    void recorrerInOrder(shared_ptr<NodoArbol> nodo, function<void(const string&, NodoPublicacion*)> callback) const {
        if (!nodo) return;

        recorrerInOrder(nodo->izquierdo, callback);
        callback(nodo->fecha, nodo->listaPublicaciones);
        recorrerInOrder(nodo->derecho, callback);
    }

    // Eliminar completamente el árbol
    void eliminarArbol() {
        eliminarNodos(raiz);
        raiz.reset();  // Aseguramos que la raíz esté vacía
    }

};

#endif // ARBOL_BINARIO_H
