#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct nodoLista {
    string emisor;
    string receptor;
    nodoLista* next;

    nodoLista(string _emisor, string _receptor) : emisor(_emisor), receptor(_receptor), next(nullptr) {}
};

struct nodoEmisor {
    string emisor;
    nodoEmisor* next;

    nodoEmisor(string _emisor) : emisor(_emisor), next(nullptr) {}
};

class Stack {
private:
    nodoLista* top;

public:
    Stack() {
        top = nullptr;
    }

    void push(string emisor, string receptor) {
        nodoLista* newNode = new nodoLista(emisor, receptor);
        if (top == nullptr) {
            top = newNode;
        } else {
            newNode->next = top;
            top = newNode;
        }
    }

    void pop() {
        if (top == nullptr) {
            cout << "La pila está vacía." << endl;
            return;
        }

        nodoLista* temp = top;
        top = top->next;
        delete temp;
    }

    string peekEmisor() {
        if (top == nullptr) {
            cout << "La pila está vacía." << endl;
            return "";
        }
        return top->emisor;
    }

    string peekReceptor() {
        if (top == nullptr) {
            cout << "La pila está vacía." << endl;
            return "";
        }
        return top->receptor;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void print() {
        nodoLista* temp = top;
        while (temp != nullptr) {
            cout << "Emisor: " << temp->emisor << " -> Receptor: " << temp->receptor << endl;
            temp = temp->next;
        }
    }

    void generateDot(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "digraph G {" << endl;
            file << "node [shape=record];" << endl;
            file << "rankdir=LR;" << endl;

            nodoLista* current = top;
            int id = 0;
            while (current != nullptr) {
                file << "node" << id << " [label=\"{Emisor: " << current->emisor << " | Receptor: " << current->receptor << "}\"];" << endl;
                if (current->next != nullptr) {
                    file << "node" << id << " -> node" << (id + 1) << ";" << endl;
                }
                current = current->next;
                id++;
            }

            file << "}" << endl;
            file.close();
        } else {
            cout << "No se pudo abrir el archivo" << endl;
        }
    }

    void renderGraphviz(const string& dotFilename, const string& imageFilename) {
        string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
        system(command.c_str());
    }

    void printSolicitudesDe(const string& receptorBuscado) {
        nodoLista* temp = top;
        cout << "Las solicitudes de " << receptorBuscado << " son:" << endl;
        bool found = false;
        while (temp != nullptr) {
            if (temp->receptor == receptorBuscado) {
                cout << "- " << temp->emisor << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No se encontraron solicitudes para " << receptorBuscado << "." << endl;
        }
    }

    nodoEmisor* retorno_emisoresDe(const string& receptorBuscado) {
        nodoLista* temp = top;
        nodoEmisor* head = nullptr;
        nodoEmisor* tail = nullptr;

        while (temp != nullptr) {
            if (temp->receptor == receptorBuscado) {
                nodoEmisor* nuevoEmisor = new nodoEmisor(temp->emisor);
                if (head == nullptr) {
                    head = nuevoEmisor;
                    tail = head;
                } else {
                    tail->next = nuevoEmisor;
                    tail = nuevoEmisor;
                }
            }
            temp = temp->next;
        }

        return head;
    }

    void removeNode(const string& emisor, const string& receptor) {
        if (top == nullptr) {
            cout << "La pila está vacía." << endl;
            return;
        }

        if (top->emisor == emisor && top->receptor == receptor) {
            nodoLista* temp = top;
            top = top->next;
            delete temp;
            cout << "Nodo eliminado en la parte superior." << endl;
            return;
        }

        nodoLista* current = top;
        nodoLista* previous = nullptr;

        while (current != nullptr && !(current->emisor == emisor && current->receptor == receptor)) {
            previous = current;
            current = current->next;
        }

        if (current != nullptr) {
            previous->next = current->next;
            delete current;
            cout << "Nodo eliminado." << endl;
        } else {
            cout << "No se encontró un nodo con el emisor y receptor especificados." << endl;
        }
    }

    void dot_solicitudes_recibidas(const string& receptorBuscado, const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "digraph G {" << endl;
            file << "node [shape=record];" << endl;
            file << "rankdir=TB;" << endl;

            nodoLista* current = top;
            int id = 0;
            while (current != nullptr) {
                if (current->receptor == receptorBuscado) {
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

        string command = "dot -Tpng " + filename + " -o " + filename.substr(0, filename.find_last_of('.')) + ".png";
        system(command.c_str());
    }

    string generarCadenaEmisorReceptor() {
        string resultado;
        nodoLista* temp = top;

        // Recorre la pila y construye la cadena con el formato "Emisor:Receptor"
        while (temp != nullptr) {
            resultado += temp->emisor + ":" + temp->receptor + "\n";  // Puedes cambiar "\n" por otro separador si lo prefieres
            temp = temp->next;
        }

        return resultado;
    }
};

#endif // STACK_H
