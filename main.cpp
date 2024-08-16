#include <iostream>
#include <fstream>
#include <cstdlib>
#include <locale>
#include <windows.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//Declaracion de variables globales

void administrador();
void user();
void cargar_usuarios();
void gestionar_usuarios();

void login() {
    cout << "Iniciando sesión..." << endl;
    cout << "Ingrese su usuario: ";
    string usuario;
    cin >> usuario;
    cout << "Ingrese su contraseña: ";
    string password;
    cin >> password;

    if (usuario == "admin@gmail.com" && password == "EDD2S2024") {
        cout << "Bienvenido, administrador" << endl;
        administrador();
    } else {
        cout << "Usuario o contraseña incorrectos" << endl;
    }
}

//Aqui empezare a crear los nodos de las estructuras de datos

struct NodeListaSimple {
    string nombres;
    string apellidos;
    string fecha_de_nacimiento;
    string correo;
    string contraseña;
    NodeListaSimple* next;

    NodeListaSimple(string _nombres, string _apellidos, string _fecha_de_nacimiento, string _correo, string _contraseña) {
        nombres = _nombres;
        apellidos = _apellidos;
        fecha_de_nacimiento = _fecha_de_nacimiento;
        correo = _correo;
        contraseña = _contraseña;
        next = nullptr;
    }
};

// Clase que define la lista simple
class ListaSimple {
private:
    NodeListaSimple* head;

public:
    ListaSimple() {
        head = nullptr;
    }

    // Agregar un nodo al final de la lista
    void append(string nombres, string apellidos, string fecha_de_nacimiento, string correo, string contraseña) {
        NodeListaSimple* newNode = new NodeListaSimple(nombres, apellidos, fecha_de_nacimiento, correo, contraseña);
        if (head == nullptr) {
            head = newNode;
        } else {
            NodeListaSimple* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Agregar un nodo al inicio de la lista
    void push(string nombres, string apellidos, string fecha_de_nacimiento, string correo, string contraseña) {
        NodeListaSimple* newNode = new NodeListaSimple(nombres, apellidos, fecha_de_nacimiento, correo, contraseña);
        if (head == nullptr) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    // Función para mostrar todos los usuarios en la lista
    void display() {
        NodeListaSimple* temp = head;
        while (temp != nullptr) {
            cout << "Nombre: " << temp->nombres << " " << temp->apellidos << endl;
            cout << "Fecha de Nacimiento: " << temp->fecha_de_nacimiento << endl;
            cout << "Correo: " << temp->correo << endl;
            cout << "Contraseña: " << temp->contraseña << endl;
            cout << "-------------------------------" << endl;
            temp = temp->next;
        }
    }

    void deleteNode(string correo) {
        NodeListaSimple* temp = head;
        NodeListaSimple* prev = nullptr;

        while (temp != nullptr && temp->correo != correo) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Usuario no encontrado" << endl;
            return;
        }

        if (prev == nullptr) {
            head = temp->next;
        } else {
            prev->next = temp->next;
        }

        delete temp;
    }
};

ListaSimple listaUsuarios;


void administrador() {
    cout << "Menú de administrador" << endl;
    cout << " ----------------------" << endl;
    cout << " 1. Carga de usuarios" << endl;
    cout << " 2. Carga de relaciones" << endl;
    cout << " 3. Carga de publicaciones" << endl;
    cout << " 4. Gestionar usuarios" << endl;
    cout << " 5. Reportes" << endl;
    cout << " 6. Salir" << endl;

    int opcion;
    cout << "Ingrese una opción: ";
    cin >> opcion;

    switch (opcion) {
        case 1:
            cargar_usuarios();
            administrador();
            break;
        case 2:
            cout << "Cargando relaciones..." << endl;
            break;
        case 3:
            cout << "Cargando publicaciones..." << endl;
            break;
        case 4:
            cout << "Gestionando usuarios..." << endl;
            gestionar_usuarios();
            break;
        case 5:
            cout << "Generando reportes..." << endl;
            break;
        case 6:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }
}

void cargar_usuarios() {
    
    ifstream file("../usuarios.json");
    json j;
    file >> j;

    for (auto& element : j) {
        string nombres = element["nombres"];
        string apellidos = element["apellidos"];
        string fecha_de_nacimiento = element["fecha_de_nacimiento"];
        string correo = element["correo"];
        string contraseña = element["contraseña"];
        listaUsuarios.append(nombres, apellidos, fecha_de_nacimiento, correo, contraseña);
    }

    listaUsuarios.display();
}

void gestionar_usuarios() {
    cout << "Menú de gestión de usuarios" << endl;
    cout << " ----------------------" << endl;
    cout << " 1. Mostrar usuarios" << endl;
    cout << " 2. Eliminar usuario" << endl;
    cout << " 3. Salir" << endl;

    int opcion;
    cout << "Ingrese una opción: ";
    cin >> opcion;

    switch (opcion) {
        case 1: {
            listaUsuarios.display();
            gestionar_usuarios();
            break;
        }
        case 2: {
            string correo;
            cout << "Ingrese el correo del usuario a eliminar: ";
            cin >> correo;
            listaUsuarios.deleteNode(correo);
            gestionar_usuarios();
            break;
        }
        case 3: {
            cout << "Saliendo..." << endl;
            administrador();
            break;
        }
        default: {
            cout << "Opción inválida" << endl;
            break;
        }
    }
}

void user() {
    cout << "Menú de usuario" << endl;
    cout << " ----------------------" << endl;
    cout << " 1. Perfil" << endl;
    cout << " 2. Solicitudes" << endl;
    cout << " 3. Publicaciones" << endl;
    cout << " 4. Reportes" << endl;
    cout << " 5. Salir" << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    cout << "Bienvenido al sistema" << endl;
    cout << "Seleccione una opción" << endl;
    cout << "----------------------" << endl;
    cout << " 1. Iniciar sesión " << endl;
    cout << " 2. Registrarse " << endl;
    cout << " 3. Información " << endl;
    cout << " 4. Salir " << endl;
    cout << endl;
    cout << "----------------------" << endl;

    int opcion;
    cout << "Ingrese una opción: ";
    cin >> opcion;

    switch (opcion) {
        case 1:
            login();
            break;
        case 2:
            cout << "Registrando usuario..." << endl;
            break;
        case 3:
            cout << "Mostrando información..." << endl;
            break;
        case 4:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }

    return 0;  // Agregar un return 0 para indicar terminación exitosa
}