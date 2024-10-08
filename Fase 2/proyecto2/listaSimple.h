#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>
using namespace std;

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

    node_amistad* getHead() const {
        return head;
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

    void dot_solicitudes_enviadas(const string &emisorBuscado, const string &filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "digraph G {" << endl;
            file << "node [shape=record];" << endl;
            file << "rankdir=LR;" << endl; // Para mostrar la lista de izquierda a derecha

            node_amistad* current = head;
            int id = 0;

            while (current != nullptr) {
                if (current->emisor == emisorBuscado) {
                    file << "node" << id << " [label=\"{Emisor: " << current->emisor << " | Receptor: " << current->receptor << "}\"];" << endl;
                    if (id > 0) {
                        file << "node" << (id - 1) << " -> node" << id << ";" << endl;
                    }
                    id++;
                }
                current = current->next;
            }

            file << "}" << endl;
            file.close();
        } else {
            cout << "No se pudo abrir el archivo" << endl;
            return;
        }

        // Convertir el archivo DOT a PNG
        string command = "dot -Tpng " + filename + " -o " + filename.substr(0, filename.find_last_of('.')) + ".png";
        system(command.c_str());
    }

    bool existeSolicitud(const string& correoEmisor, const string& correoReceptor) const {
        node_amistad* temp = head;
        while (temp != nullptr) {
            // Verifica si hay una solicitud entre el emisor y receptor deseado
            if ((temp->emisor == correoEmisor && temp->receptor == correoReceptor) ||
                (temp->emisor == correoReceptor && temp->receptor == correoEmisor)) {
                return true;  // Hay una solicitud existente
            }
            temp = temp->next;
        }
        return false; // No hay solicitudes existentes
    }
};

#endif // LISTASIMPLE_H
