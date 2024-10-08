#ifndef APPDATA_H
#define APPDATA_H


#include "AVL.h"
#include "doublylinkedlist.h"
#include "stack.h"
#include "listaSimple.h"
#include "matrix.h"
#include "arbolBinario.h"

class AppData {
private:
    AVL avlTree;  // Asumiendo que tienes un AVL aquí
    DoublyLinkedList listaDoble;
    Stack pilaReceptor;
    ListaAmistad listaEmisor;
    Matrix matrizAmistad;

    ArbolBinario arbolesDePublicaciones;


    // Constructor privado para evitar la creación directa
    AppData() {}

public:
    // Método estático para obtener la instancia única de AppData
    static AppData& getInstance() {
        static AppData instance;  // Instancia única
        return instance;
    }

    // Evitar copia y asignación
    AppData(AppData const&) = delete;
    void operator=(AppData const&) = delete;

    // Método público para acceder al árbol AVL
    AVL& getAVLTree() {
        return avlTree;
    }

    DoublyLinkedList& getListaDoble(){
        return listaDoble;
    }

    Stack& getPilaReceptor(){
        return pilaReceptor;
    }

    ListaAmistad& getListaEmisor(){
        return listaEmisor;
    }

    Matrix& getMatrizAmistad(){
        return matrizAmistad;
    }

    // Método para obtener el árbol binario de publicaciones de un usuario

    ArbolBinario& getArbolDePublicaciones() {
        return arbolesDePublicaciones;
    }


};


#endif // APPDATA_H
