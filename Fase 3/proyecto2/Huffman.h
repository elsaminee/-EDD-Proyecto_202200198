#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <string>
#include <cstring> // Para memset y strcpy
#include <stdexcept> // Para excepciones
#include <fstream>
#include <sys/stat.h> // Para verificar si el archivo existe
#include <QBuffer>
#include <sstream>  // Para std::stringstream


class Huffman {
private:
    class Nodo {
    public:
        char caracter;
        int frecuencia;
        Nodo* siguiente;
        Nodo* izquierdo;
        Nodo* derecho;

        Nodo(char c, int f) : caracter(c), frecuencia(f), siguiente(nullptr), izquierdo(nullptr), derecho(nullptr) {}
    };

    class ListaEnlazadaHuff {
    private:
        Nodo* cabeza;

    public:
        ListaEnlazadaHuff() : cabeza(nullptr) {}

        ~ListaEnlazadaHuff() {
            // Liberar la memoria de los nodos restantes
            while (cabeza) {
                Nodo* temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
        }

        void insertarEnOrden(Nodo* nuevoNodo) {
            if (!cabeza || nuevoNodo->frecuencia < cabeza->frecuencia) {
                nuevoNodo->siguiente = cabeza;
                cabeza = nuevoNodo;
            } else {
                Nodo* actual = cabeza;
                while (actual->siguiente && actual->siguiente->frecuencia <= nuevoNodo->frecuencia) {
                    actual = actual->siguiente;
                }
                nuevoNodo->siguiente = actual->siguiente;
                actual->siguiente = nuevoNodo;
            }
        }

        Nodo* extraerMinimo() {
            if (!cabeza) return nullptr;
            Nodo* minimo = cabeza;
            cabeza = cabeza->siguiente;
            return minimo;
        }

        bool soloUno() {
            return cabeza && !cabeza->siguiente;
        }

        Nodo* obtenerCabeza() {
            return cabeza;
        }
    };

    Nodo* root;
    char* tablaCaracteres[128];
    int tamanioCodigo[128];

    void construirArbol(const std::string& texto) {
        if (texto.empty()) {
            throw std::runtime_error("Error: El texto de entrada está vacío.");
        }

        int frecuencias[128] = {0};

        // Contar las frecuencias de cada carácter
        for (char c : texto) {
            frecuencias[(int)c]++;
        }

        // Crear una lista enlazada con los nodos correspondientes
        ListaEnlazadaHuff lista;
        bool tieneCaracteres = false;  // Variable para verificar si al menos un carácter tiene frecuencia > 0
        for (int i = 0; i < 128; ++i) {
            if (frecuencias[i] > 0) {
                tieneCaracteres = true;
                Nodo* nuevoNodo = new Nodo((char)i, frecuencias[i]);
                lista.insertarEnOrden(nuevoNodo);
            }
        }

        // Si no hay caracteres con frecuencia > 0, el árbol no puede construirse
        if (!tieneCaracteres) {
            throw std::runtime_error("Error: El texto de entrada no tiene caracteres válidos.");
        }

        // Construir el árbol de Huffman
        while (!lista.soloUno()) {
            Nodo* izq = lista.extraerMinimo();
            Nodo* der = lista.extraerMinimo();
            Nodo* nuevo = new Nodo('\0', izq->frecuencia + der->frecuencia);
            nuevo->izquierdo = izq;
            nuevo->derecho = der;
            lista.insertarEnOrden(nuevo);
        }

        root = lista.obtenerCabeza();  // La raíz será el único nodo restante

        if (!root) {
            throw std::runtime_error("Error: El árbol de Huffman no se construyó correctamente. Root es nullptr.");
        }
    }

    void generarCodigos(Nodo* nodo, char* codigo, int longitud) {
        if (!nodo) return;

        if (!nodo->izquierdo && !nodo->derecho) {
            codigo[longitud] = '\0';  // Termina el código
            tablaCaracteres[(int)nodo->caracter] = new char[longitud + 1];
            tamanioCodigo[(int)nodo->caracter] = longitud;
            strcpy(tablaCaracteres[(int)nodo->caracter], codigo);

            std::cout << "Character: " << nodo->caracter << ", Code: " << tablaCaracteres[(int)nodo->caracter] << std::endl;
        }

        if (nodo->izquierdo) {
            codigo[longitud] = '0';
            generarCodigos(nodo->izquierdo, codigo, longitud + 1);
        }

        if (nodo->derecho) {
            codigo[longitud] = '1';
            generarCodigos(nodo->derecho, codigo, longitud + 1);
        }
    }

    std::string codificar(const std::string& texto) {
        std::string textoCodificado;
        for (char c : texto) {
            textoCodificado += tablaCaracteres[(int)c];
        }
        return textoCodificado;
    }

    std::string decodificar(const std::string& textoCodificado) {
        std::string textoDecodificado;
        Nodo* actual = root; // Comenzar desde la raíz

        if (!root) {
            throw std::runtime_error("Error: El árbol de Huffman no se ha construido correctamente.");
        }

        std::cout << "Comienza la descompresión..." << std::endl;

        for (char c : textoCodificado) {
            // Verificar si el nodo actual es nulo
            if (!actual) {
                std::cerr << "Error: Nodo actual es nulo. Posible inconsistencia en el árbol o en el texto codificado." << std::endl;
                return "";
            }

            // Navegar el árbol de Huffman según el bit
            if (c == '0') {
                actual = actual->izquierdo;
            } else if (c == '1') {
                actual = actual->derecho;
            } else {
                std::cerr << "Error: Caracter inválido en el texto codificado: " << c << std::endl;
                return ""; // Regresar cadena vacía si se encuentra un carácter no válido
            }

            // Si llegamos a un nodo hoja, agregamos el carácter decodificado al resultado
            if (!actual->izquierdo && !actual->derecho) {
                textoDecodificado += actual->caracter;
                actual = root; // Reiniciar al nodo raíz para el próximo carácter
            }
        }

        // Comprobamos si terminamos en un nodo no hoja (incompleto)
        if (actual != root) {
            std::cerr << "Error: Texto codificado incompleto, terminó en un nodo no hoja." << std::endl;
            return "";
        }

        std::cout << "Descompresión completada correctamente" << std::endl;
        return textoDecodificado;
    }


public:
    Huffman() : root(nullptr) {
        memset(tablaCaracteres, 0, sizeof(tablaCaracteres));
    }

    ~Huffman() {
        for (int i = 0; i < 128; ++i) {
            if (tablaCaracteres[i]) {
                delete[] tablaCaracteres[i];
            }
        }
    }

    std::string comprimir(const std::string& texto) {
        construirArbol(texto);
        char codigo[128];
        generarCodigos(root, codigo, 0);
        return codificar(texto);
    }

    std::string descomprimir(const std::string& textoCodificado) {
        codificar(textoCodificado);
        return textoCodificado;
    }

    void guardarArchivoComprimido(const std::string& nombreArchivo, const std::string& textoCodificado) {
        std::ofstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo) {
            throw std::runtime_error("No se pudo abrir el archivo para guardar.");
        }

        // Guardar tabla de códigos de Huffman
        for (int i = 0; i < 128; ++i) {
            if (tablaCaracteres[i] != nullptr) {
                archivo << (char)i << " " << tablaCaracteres[i] << "\n";
            }
        }

        // Separador para indicar el fin de la tabla de códigos
        archivo << "---\n";

        // Guardar texto codificado
        archivo << textoCodificado;

        archivo.close();
    }
    std::string descomprimirArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo, std::ios::binary); // Abrir en modo binario

        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
        }

        std::stringstream buffer;
        buffer << archivo.rdbuf(); // Leer todo el contenido del archivo en el buffer

        archivo.close();
        std::string contenidoArchivo = buffer.str(); // Convertir el contenido a string

        // Ahora comprimimos el contenido utilizando el método de Huffman
        std::string contenidoComprimido = comprimir(contenidoArchivo);

        return contenidoArchivo; // Retornamos el contenido comprimido
    }

};

#endif
