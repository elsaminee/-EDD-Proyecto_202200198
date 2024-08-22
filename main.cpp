#include <iostream>
#include <fstream>
#include <cstdlib>
#include <locale>
#include <windows.h>
#include "json.hpp"
#include "matrix.h"

using namespace std;
using json = nlohmann::json;

void menu_principal();
void administrador();
void cargar_usuarios();
void gestionar_usuarios();
void cargar_relaciones();
void cargar_publicaciones();
void reportes();

//Aqui empezare a crear los nodos de las estructuras de datos

struct nodoEmisor
{
    string emisor;
    nodoEmisor *next;

    nodoEmisor(string _emisor)
    {
        emisor = _emisor;
        next = nullptr;
    }
};

struct nodoLista
{
    string emisor;
    string receptor;
    nodoLista *next;

    nodoLista(string _emisor, string _receptor)
    {
        emisor = _emisor;
        receptor = _receptor;
        next = nullptr;
    }
};

class Stack
{
private:
    nodoLista *top;

public:
    Stack()
    {
        top = nullptr;
    }

    void push(string emisor, string receptor)
    {
        nodoLista *newNode = new nodoLista(emisor, receptor);
        if (top == nullptr)
        {
            top = newNode;
        }
        else
        {
            newNode->next = top;
            top = newNode;
        }
    }

    void pop()
    {
        if (top == nullptr)
        {
            cout << "La pila está vacía." << endl;
            return;
        }

        nodoLista *temp = top;
        top = top->next;
        delete temp;
    }

    string peekEmisor()
    {
        if (top == nullptr)
        {
            cout << "La pila está vacía." << endl;
            return "";  // Devolver una cadena vacía en lugar de un valor de error
        }
        return top->emisor;
    }

    string peekReceptor()
    {
        if (top == nullptr)
        {
            cout << "La pila está vacía." << endl;
            return "";  // Devolver una cadena vacía en lugar de un valor de error
        }
        return top->receptor;
    }

    bool isEmpty()
    {
        return top == nullptr;
    }

    void print()
    {
        nodoLista *temp = top;
        while (temp != nullptr)
        {
            cout << "Emisor: " << temp->emisor << " -> Receptor: " << temp->receptor << endl;
            temp = temp->next;
        }

    }

    void generateDot(const string &filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            file << "digraph G {" << endl;
            file << "node [shape=record];" << endl;
            file << "rankdir=LR;" << endl;

            nodoLista *current = top;
            int id = 0;
            while (current != nullptr)
            {
                file << "node" << id << " [label=\"{Emisor: " << current->emisor << " | Receptor: " << current->receptor << "}\"];" << endl;
                if (current->next != nullptr)
                {
                    file << "node" << id << " -> node" << (id + 1) << ";" << endl;
                }
                current = current->next;
                id++;
            }

            file << "}" << endl;
            file.close();
        }
        else
        {
            cout << "No se pudo abrir el archivo" << endl;
        }
    }

    void renderGraphviz(const string& dotFilename, const string& imageFilename) {
        string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
        system(command.c_str());
    }

    void printSolicitudesDe(const string &receptorBuscado)
    {
        nodoLista *temp = top;
        cout << "Las solicitudes de " << receptorBuscado << " son:" << endl;
        bool found = false;
        while (temp != nullptr)
        {
            if (temp->receptor == receptorBuscado)
            {
                cout << "- " << temp->emisor << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found)
        {
            cout << "No se encontraron solicitudes para " << receptorBuscado << "." << endl;
        }
    }

    nodoEmisor* retorno_emisoresDe(const string &receptorBuscado)
    {
        nodoLista *temp = top;
        nodoEmisor *head = nullptr;  // Cabeza de la lista de emisores
        nodoEmisor *tail = nullptr;  // Cola de la lista de emisores

        while (temp != nullptr)
        {
            if (temp->receptor == receptorBuscado)
            {
                nodoEmisor *nuevoEmisor = new nodoEmisor(temp->emisor);
                if (head == nullptr)
                {
                    head = nuevoEmisor;  // Inicializa la cabeza de la lista
                    tail = head;          // Inicializa la cola
                }
                else
                {
                    tail->next = nuevoEmisor;  // Añade al final de la lista
                    tail = nuevoEmisor;        // Actualiza la cola
                }
            }
            temp = temp->next;
        }

        return head;
    }

    void removeNode(const string &emisor, const string &receptor)
    {
        if (top == nullptr)
        {
            cout << "La pila está vacía." << endl;
            return;
        }

        // Caso especial: si el nodo a eliminar está en la parte superior
        if (top->emisor == emisor && top->receptor == receptor)
        {
            nodoLista *temp = top;
            top = top->next;
            delete temp;
            cout << "Nodo eliminado en la parte superior." << endl;
            return;
        }

        nodoLista *current = top;
        nodoLista *previous = nullptr;

        // Recorrer la pila para encontrar el nodo que coincida con los criterios
        while (current != nullptr && !(current->emisor == emisor && current->receptor == receptor))
        {
            previous = current;
            current = current->next;
        }

        // Si se encontró el nodo, eliminarlo
        if (current != nullptr)
        {
            previous->next = current->next;
            delete current;
            cout << "Nodo eliminado." << endl;
        }
        else
        {
            cout << "No se encontró un nodo con el emisor y receptor especificados." << endl;
        }
    }
};


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

    bool correo_existente(string correo) {
        NodeListaSimple* temp = head;
        while (temp != nullptr) {
            if (temp->correo == correo) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Agregar un nodo al final de la lista
    void append(string nombres, string apellidos, string fecha_de_nacimiento, string correo, string contraseña) {

        if(correo_existente(correo)) {
            cout << "El correo ya está registrado" << endl;
            return;
        }

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

    bool verificarCredenciales(string correo, string contraseña) {
        NodeListaSimple* temp = head;
        while (temp != nullptr) {
            if (temp->correo == correo && temp->contraseña == contraseña) {
                return true;
            }
            temp = temp->next;
        }
        return false;
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

    void info_usuario(string correo) {
        NodeListaSimple* temp = head;
        while (temp != nullptr) {
            if (temp->correo == correo) {
                cout << "Nombre: " << temp->nombres << " " << temp->apellidos << endl;
                cout << "Fecha de Nacimiento: " << temp->fecha_de_nacimiento << endl;
                cout << "Correo: " << temp->correo << endl;
                cout << "Contraseña: " << temp->contraseña << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Usuario no encontrado" << endl;
    }

    void generateDot() {
        ofstream file("usuarios.dot");
        if (file.is_open()) {
            file << "digraph G {" << endl;
            file << "node [shape=record];" << endl;
            file << "rankdir=LR;" << endl;

            NodeListaSimple* current = head;
            int id = 0;
            while (current != nullptr) {
                file << "node" << id << " [label=\"{Nombre: " << current->nombres << " " << current->apellidos << " | Correo: " << current->correo << "}\"];" << endl;
                if (current->next != nullptr) {
                    file << "node" << id << " -> node" << (id + 1) << ";" << endl;
                }
                current = current->next;
                id++;
            }

            file << "}" << endl;
            file.close();

            string command = "dot -Tpng usuarios.dot -o usuarios.png";
            system(command.c_str());
        } else {
            cout << "No se pudo abrir el archivo" << endl;
        }

    }
};

struct nodo_aceptada {
    string emisor;
    string receptor;
    nodo_aceptada* next;

    nodo_aceptada(string _emisor, string _receptor) {
        emisor = _emisor;
        receptor = _receptor;
        next = nullptr;
    }
};

class ListaAceptada {
private:
    nodo_aceptada* head;

public:
    ListaAceptada() {
        head = nullptr;
    }

    // Verificar si el receptor ya existe en la lista
    bool receptor_existente(string receptor) {
        nodo_aceptada* temp = head;
        while (temp != nullptr) {
            if (temp->receptor == receptor) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Agregar un nodo a la lista si el receptor no está repetido
    void append(string emisor, string receptor) {
        if (receptor_existente(receptor)) {
            cout << "El receptor ya no es posible enviarle solicitud" << endl;	
            return;
        }

        nodo_aceptada* newNode = new nodo_aceptada(emisor, receptor);
        if (head == nullptr) {
            head = newNode;
        } else {
            nodo_aceptada* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void removeIfMatch(string emisor, string receptor) {
        if (head == nullptr) {
            return;
        }

        // Caso especial: si el nodo a eliminar está en la cabeza de la lista
        if (head->emisor == emisor && head->receptor == receptor) {
            nodo_aceptada* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        nodo_aceptada* current = head;
        nodo_aceptada* previous = nullptr;

        // Recorrer la lista para encontrar el nodo que coincida con los criterios
        while (current != nullptr && !(current->emisor == emisor && current->receptor == receptor)) {
            previous = current;
            current = current->next;
        }

        // Si se encontró el nodo, eliminarlo
        if (current != nullptr) {
            previous->next = current->next;
            delete current;
        }
    }

    // Mostrar todos los nodos de la lista
    void display() {
        nodo_aceptada* temp = head;
        while (temp != nullptr) {
            cout << "Emisor: " << temp->emisor << " -> Receptor: " << temp->receptor << endl;
            temp = temp->next;
        }
    }
};


struct node_amistad {
    string emisor;
    string receptor;
    node_amistad* next;

    node_amistad(string _emisor, string _receptor) {
        emisor = _emisor;
        receptor = _receptor;
        next = nullptr;
    }
};

class ListaAmistad {
private:
    node_amistad* head;

public:
    ListaAmistad() {
        head = nullptr;
    }

    void append(string emisor, string receptor) {
        node_amistad* newNode = new node_amistad(emisor, receptor);
        if (head == nullptr) {
            head = newNode;
        } else {
            node_amistad* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() {
        node_amistad* temp = head;
        while (temp != nullptr) {
            cout << "Emisor: " << temp->emisor << " -> Receptor: " << temp->receptor << endl;
            temp = temp->next;
        }
    }

    void deleteNode(string emisor, string receptor) {
        node_amistad* temp = head;
        node_amistad* prev = nullptr;

        while (temp != nullptr && (temp->emisor != emisor || temp->receptor != receptor)) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Relación no encontrada" << endl;
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


struct node_publi {
    int id;
    string correo;
    string contenido_correo;
    string fecha;
    string hora;
    node_publi* next;
    node_publi* prev;

    node_publi(int id, string correo, string contenido_correo, string fecha, string hora) {
        this->id = id;
        this->correo = correo;
        this->contenido_correo = contenido_correo;
        this->fecha = fecha;
        this->hora = hora;
        next = nullptr;
        prev = nullptr;
    }
};

class DoublyLinkedList {
private:
    node_publi* head;
    node_publi* tail;
    int nextId; // Para asignar IDs únicos

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), nextId(0) {}

    void append(string correo, string contenido_correo, string fecha, string hora) {
        node_publi* newNode = new node_publi(nextId++, correo, contenido_correo, fecha, hora);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void push(string correo, string contenido_correo, string fecha, string hora) {
        node_publi* newNode = new node_publi(nextId++, correo, contenido_correo, fecha, hora);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void print() {
        node_publi* current = head;
        while (current != nullptr) {
            cout << "ID: " << current->id << " | Correo: " << current->correo
                 << " | Contenido: " << current->contenido_correo
                 << " | Fecha: " << current->fecha << " | Hora: " << current->hora << " <-> ";
            current = current->next;
        }
        cout << "fin" << endl;
    }

    void remove(string correo) {
        node_publi* current = head;

        while (current != nullptr && current->correo != correo) {
            current = current->next;
        }

        if (current == nullptr) {
            cout << "No se encontró el correo a eliminar" << endl;
            return;
        }

        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }

        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }

        delete current;
    }

    bool search(string correo) {
        node_publi* current = head;
        while (current != nullptr) {
            if (current->correo == correo) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void graph() {
        ofstream file("graph.dot");
        file << "digraph G {" << endl;
        file << "rankdir=LR;" << endl;
        file << "node [shape=record];" << endl;

        node_publi* current = head;
        while (current != nullptr) {
            file << "node" << current->id << " [label=\"{ID: " << current->id
                << " | Correo: " << current->correo
                << " | Contenido: " << current->contenido_correo
                << " | Fecha: " << current->fecha
                << " | Hora: " << current->hora << "}\"];" << endl;
            if (current->next != nullptr) {
                file << "node" << current->id << " -> node" << current->next->id << " ;" << endl;
                file << "node" << current->next->id << " -> node" << current->id << " ;" << endl;
            }
            current = current->next;
        }

        file << "}" << endl;
        file.close();

        string command = "dot -Tpng publi.dot -o publi.png";
        system(command.c_str());
    }

    void removeById(int id) {
        node_publi* current = head;

        // Buscar el nodo con el id correspondiente
        while (current != nullptr && current->id != id) {
            current = current->next;
        }

        // Si no se encontró el nodo
        if (current == nullptr) {
            cout << "No se encontró el nodo con ID: " << id << endl;
            return;
        }

        // Si el nodo a eliminar no es el primero
        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else { 
            // Si el nodo a eliminar es el primero
            head = current->next;
        }

        // Si el nodo a eliminar no es el último
        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else { 
            // Si el nodo a eliminar es el último
            tail = current->prev;
        }

        delete current;
        cout << "Nodo con ID: " << id << " eliminado" << endl;
    }

    ~DoublyLinkedList() {
        node_publi* current = head;
        while (current != nullptr) {
            node_publi* next = current->next;
            delete current;
            current = next;
        }
    }

    void printByUser(string correo) {
        node_publi* current = head;
        bool found = false;

        while (current != nullptr) {
            if (current->correo == correo) {
                cout << "ID: " << current->id << " | Correo: " << current->correo
                     << " | Contenido: " << current->contenido_correo
                     << " | Fecha: " << current->fecha << " | Hora: " << current->hora << endl;
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No se encontraron publicaciones para el correo: " << correo << endl;
        }
    }

};


// Estructuras para la matriz dispersa
//-----------------------------------------------------
//-----------------------------------------------------

//Bueno se hizo en otro archivo xddd


//Declaracion de las estructuras globales
//-----------------------------------------------------
//-----------------------------------------------------
ListaSimple listaUsuarios;
Stack pila_relaciones;
ListaAmistad listaAmistades;
Matrix matrix_amistades;
DoublyLinkedList listaPublicaciones;



void registro() {
    cout << "Registrando usuario..." << endl;
    cout << "Ingrese su nombre: ";
    string nombres;
    cin >> nombres;
    cout << "Ingrese su apellido: ";
    string apellidos;
    cin >> apellidos;
    cout << "Ingrese su fecha de nacimiento" << endl;
    cout << "Formato YYYY/MM/DD: " ;
    string fecha_de_nacimiento;
    cin >> fecha_de_nacimiento;
    cout << "Ingrese su correo: ";
    string correo;
    cin >> correo;
    cout << "Ingrese su contraseña: ";
    string contraseña;
    cin >> contraseña;

    listaUsuarios.append(nombres, apellidos, fecha_de_nacimiento, correo, contraseña);
    menu_principal();
}

void user(string correo) {
    int opcion;
    string correo_usuario = correo;
    do {
        cout << "Menú de usuario" << endl;
        cout << " ----------------------" << endl;
        cout << " 1. Perfil" << endl;
        cout << " 2. Solicitudes" << endl;
        cout << " 3. Publicaciones" << endl;
        cout << " 4. Reportes" << endl;
        cout << " 5. Salir" << endl;

        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int opcion_perfil;
                cout << "1. Ver Perfil" << endl;
                cout << "2. Eliminar Cuenta" << endl;
                
                cout << "Ingrese una opción: ";
                cin >> opcion_perfil;

                switch (opcion_perfil) {
                    case 1:
                        listaUsuarios.info_usuario(correo);
                        break;
                    case 2:
                        listaUsuarios.deleteNode(correo);
                        menu_principal();
                        return; // Importante para salir de la función después de llamar a menu_principal()
                    default:
                        cout << "Opción inválida" << endl;
                        break;
                }
                break;
            }
            case 2: {
                int opcion_solicitudes;

                do {
                    cout << "Menu solicitudes" << endl;
                    cout << "1. Ver solicitudes" << endl;
                    cout << "2. Enviar solicitud" << endl;
                    cout << "3. Salir" << endl;
                    cout << "Ingrese una opción de menu: ";
                    cin >> opcion_solicitudes;

                    if (opcion_solicitudes == 1) {
                        // Se visualizan las solicitudes que tiene el receptor
                        pila_relaciones.printSolicitudesDe(correo_usuario);
                        nodoEmisor* emisores = pila_relaciones.retorno_emisoresDe(correo_usuario);
                        nodoEmisor* current = emisores;

                        while (current != nullptr) {
                            cout << "Emisor: " << current->emisor << endl;
                            cout << "1. Aceptar" << endl;
                            cout << "2. Rechazar" << endl;
                            cout << "3. Salir" << endl;

                            int opcion_aceptar_rechazar;
                            cout << "Ingrese una opción: ";
                            cin >> opcion_aceptar_rechazar;

                            switch (opcion_aceptar_rechazar) {
                                case 1:
                                    matrix_amistades.insert(current->emisor, correo_usuario);
                                    pila_relaciones.removeNode(current->emisor, correo_usuario);
                                    listaAmistades.deleteNode(current->emisor, correo_usuario);
                                    break;
                                case 2:
                                    pila_relaciones.removeNode(current->emisor, correo_usuario);
                                    listaAmistades.deleteNode(current->emisor, correo_usuario);
                                    break;
                                case 3:
                                    cout << "Saliendo..." << endl;
                                    user(correo_usuario);
                                    break;
                                default:
                                    cout << "Opción inválida" << endl;
                                    break;
                            }

                            current = current->next;
                        }

                    } else if (opcion_solicitudes == 2) {
                        string receptor;
                        cout << "Ingrese el correo del receptor: ";
                        cin >> receptor;

                        if (listaUsuarios.correo_existente(receptor)) {
                            pila_relaciones.push(correo_usuario, receptor);
                            listaAmistades.append(correo_usuario, receptor);
                            cout << "Solicitud enviada" << endl;
                        } else {
                            cout << "El correo no está registrado" << endl;
                        }
                    }

                } while (opcion_solicitudes != 3);

                break;
            }
            case 3: {
                int opcion_publicaciones;
                string contenido_correo;
                string fecha_publicacion;
                string hora_publicacion;

                do {
                    cout << "Menú de publicaciones" << endl;
                    cout << "1. Ver publicaciones" << endl;
                    cout << "2. Crear publicación" << endl;
                    cout << "3. Eliminar publicación" << endl;
                    cout << "4. Salir" << endl;

                    cout << "Ingrese una opción: ";
                    cin >> opcion_publicaciones;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    if (opcion_publicaciones == 1) {
                        // Mostrar las publicaciones del usuario siendo amigos
                        listaPublicaciones.print();
                    } else if (opcion_publicaciones == 2) {
                        cout << "Ingrese el contenido de la publicación: ";
                        cin >> contenido_correo;
                        cout << "Ingresa la fecha de la publicación (DD/MM/YYYY): ";
                        cin >> fecha_publicacion;
                        cout << "Ingresa la hora de la publicación (HH:MM): ";
                        cin >> hora_publicacion;
                        listaPublicaciones.append(correo_usuario, contenido_correo, fecha_publicacion, hora_publicacion);

                    } else if (opcion_publicaciones == 3) {
                        int id;
                        listaPublicaciones.printByUser(correo_usuario);
                        cout << "Ingrese el ID de la publicación a eliminar: ";
                        
                        cin >> id;
                        listaPublicaciones.removeById(id);
                    } else if (opcion_publicaciones == 4) {
                        cout << "Saliendo..." << endl;
                        user(correo_usuario);
                    } else {
                        cout << "Opción inválida" << endl;
                    }
                } while (opcion_publicaciones != 4);

                break;
            }
            case 4:
                cout << "Generando reportes..." << endl;
                break;
            case 5:
                cout << "Saliendo..." << endl;
                menu_principal();
                return; // Importante para salir de la función después de llamar a menu_principal()
            default:
                cout << "Opción inválida" << endl;
                break;
        }
    } while (opcion != 5);

    menu_principal();
}


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
    } if(listaUsuarios.verificarCredenciales(usuario, password)) {
        cout << "Bienvenido, usuario" << endl;
        user(usuario);
    } else {
        cout << "Credenciales incorrectas" << endl;
        menu_principal();
    }
}

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
            cargar_relaciones();
            administrador();
            break;
        case 3:
            cargar_publicaciones();
            administrador();
            break;
        case 4:
            cout << "Gestionando usuarios..." << endl;
            gestionar_usuarios();
            break;
        case 5:
            reportes();
            break;
        case 6:
            cout << "Saliendo..." << endl;
            menu_principal();
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }
}

void reportes() {
    cout << "1. Reportes usuarios" << endl;
    cout << "2. Reportes relaciones" << endl;
    cout << "3. Reportes publicaciones" << endl;
    cout << "4. Tops" << endl;
    cout << "5. Salir" << endl;

    int opcion = 0;

    do
    {
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            listaUsuarios.generateDot();
            break;
        case 2:
            matrix_amistades.generateDot("amistad.dot");
            break;
        case 3:
            listaPublicaciones.graph();
            break;
        case 4:
            cout << "Tops" << endl;
            cout << "1. Top 5 usuarios con más publicaciones" << endl;
            cout << "2. Top 5 usuarios con menos amigos" << endl;
            cout << "3. Salir" << endl;

            int opcion_top;
            cout << "Ingrese una opción: ";
            cin >> opcion_top;

            do
            {
                cout << "Ingrese una opción: ";
                cin >> opcion_top;
            } while (opcion_top != 3);
            
            break;
        case 5:
            administrador();
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
        }
    } while (opcion != 5);

}

void cargar_usuarios() {
    
    cout << "Ingrese el nombre del archivo de usuarios: ";
    string filename;
    cin >> filename;

    string filepath = "../" + filename;
    ifstream file(filepath);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

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

void cargar_relaciones() {
    cout << "Cargando relaciones..." << endl;
    cout << "Ingrese el nombre del archivo de relaciones: ";
    string filename;
    cin >> filename;

    string filepath = "../" + filename;
    ifstream file(filepath);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    json j;
    file >> j;

    for (auto& element : j) {
        string receptor = element["receptor"];
        string emisor = element["emisor"];
        string estado = element["estado"];
        if (estado == "PENDIENTE") {
            pila_relaciones.push(emisor, receptor);
            listaAmistades.append(emisor, receptor);
        }
        if (estado == "ACEPTADA") {
            matrix_amistades.insert(emisor, receptor);
        }
        if (estado == "RECHAZADA") {
            cout << "Falta programar esto" << endl;
        }
    }

    pila_relaciones.print();
}

void cargar_publicaciones() {
    cout << "Ingrese el nombre del archivo de publicaciones: ";
    string filename;
    cin >> filename;

    string filepath = "../" + filename;
    ifstream file(filepath);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    json j;
    file >> j;

    for (auto& element : j) {
        string correo = element["correo"];
        string contenido_correo = element["contenido"];
        string fecha = element["fecha"];
        string hora = element["hora"];
        listaPublicaciones.append(correo, contenido_correo, fecha, hora);
        cout <<  "El correo es: " << correo << endl;
        cout << "El contenido es: " << contenido_correo << endl;
    }
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



int main() {
    SetConsoleOutputCP(CP_UTF8);
    menu_principal();

    return 0;  // Agregar un return 0 para indicar terminación exitosa
}

void menu_principal() {
    int opcion = 0;
    do {
        cout << "Bienvenido al sistema" << endl;
        cout << "Seleccione una opción" << endl;
        cout << "----------------------" << endl;
        cout << " 1. Iniciar sesión " << endl;
        cout << " 2. Registrarse " << endl;
        cout << " 3. Información " << endl;
        cout << " 4. Salir " << endl;
        cout << "----------------------" << endl;

        cout << "Ingrese una opción: ";
        cin >> opcion;

        // Verificar si la entrada es válida
        if (cin.fail()) {
            cin.clear(); // Limpia el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada no válida
            cout << "Opción inválida. Intente de nuevo." << endl;
            continue; // Vuelve a mostrar el menú
        }

        switch (opcion) {
            case 1:
                login();
                break;
            case 2:
                registro();
                break;
            case 3:
                cout << "Mostrando información..." << endl;
                cout << "Nombre: Samuel Nehemias Coyoy Perez" << endl;
                cout << "Carnet: 202200198" << endl;
                cout << "Curso: Estructura de Datos Seccion C " << endl;
                break;
            case 4:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 4);
}