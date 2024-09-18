#ifndef APPDATA_H
#define APPDATA_H


#include "AVL.h"

class AppData {
private:
    AVL avlTree;  // Asumiendo que tienes un AVL aquí

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
};


#endif // APPDATA_H
