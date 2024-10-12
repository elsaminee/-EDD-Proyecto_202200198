#ifndef APPDATA_H
#define APPDATA_H


#include "AVL.h"
#include "doublylinkedlist.h"
#include "stack.h"
#include "listaSimple.h"
#include "arbolBinario.h"
#include "grafoNdirigido.h"


class AppData {
private:
    AVL avlTree;
    DoublyLinkedList listaDoble;
    Stack pilaReceptor;
    ListaAmistad listaEmisor;
    ListaAdyacencia grafo;
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

    // Método para obtener el árbol binario de publicaciones de un usuario

    ArbolBinario& getArbolDePublicaciones() {
        return arbolesDePublicaciones;
    }

    ListaAdyacencia& getGrafo() {
        return grafo;
    }


};


#endif // APPDATA_H
