#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>
#include <QCryptographicHash>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <QString>
#include <fstream>  // Para manejar archivos
#include <filesystem>
#include <QDebug>

using namespace std;

// Clase que representa un Bloque en la Blockchain
class Block {
public:
    int index;
    string timestamp;
    string data;
    string previousHash;
    string hash;
    int nonce; // Nuevo campo para la minería

    Block(int idx, const string& data, const string& prevHash)
        : index(idx), data(data), previousHash(prevHash), nonce(0), timestamp(getCurrentTime()) {
        // Si es el bloque génesis, forzar previousHash a "0000"
        if (index == 1) {
            previousHash = "0000";
        }
        hash = calculateHash();  // Calcular el hash inicial del bloque
    }
    // Función para obtener la hora actual en el formato requerido
    string getCurrentTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << setw(2) << setfill('0') << ltm->tm_mday << "-"
           << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
           << 1900 + ltm->tm_year << "::"
           << setw(2) << setfill('0') << ltm->tm_hour << ":"
           << setw(2) << setfill('0') << ltm->tm_min << ":"
           << setw(2) << setfill('0') << ltm->tm_sec;
        return ss.str();
    }

    void saveToJSON() {
        const std::string folderName = "bloques";
        std::filesystem::create_directory(folderName);
        std::string fileName = folderName + "/block_" + std::to_string(index) + ".json";
        std::ofstream file(fileName);

        if (!file.is_open()) {
            std::cerr << "Error al abrir el archivo " << fileName << std::endl;
            return; // Salir si no se pudo abrir el archivo
        }

        file << "{\n";
        file << "  \"INDEX\": " << index << ",\n";
        file << "  \"TIMESTAMP\": \"" << timestamp << "\",\n";
        file << "  \"NONCE\": \"" << nonce << "\",\n";
        file << "   " <<  data << ",\n"; // Solo guardamos un objeto de publicación
        file << "  \"PREVIOUSHASH\": \"" << previousHash << "\",\n";
        file << "  \"HASH\": \"" << hash << "\"\n";
        file << "}\n";

        file.close();
        std::cout << "Bloque guardado como " << fileName << std::endl;
    }

    string calculateHash() {
        QString input = QString::fromStdString(to_string(index) + timestamp + data + previousHash + to_string(nonce)); // Incluir nonce
        QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256);
        return QString(hash.toHex()).toStdString();
    }

    void mineBlock(int difficulty) {
        string target(difficulty, '0'); // Crear un string con 'difficulty' ceros

        // Para otros bloques, calcula el hash usando el previousHash
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
        cout << "Bloque minado: " << hash << endl;
    }
};

// Clase que representa la Blockchain como una lista enlazada simple
class Blockchain {
private:
    struct Node {
        Block* block;
        Node* next;
        Node(Block* blk) : block(blk), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int difficulty;

public:
    Blockchain(int diff) : head(nullptr), tail(nullptr), difficulty(diff) {
        Block* genesisBlock = new Block(0, "{}", "0000"); // Bloque génesis
        genesisBlock->mineBlock(difficulty);
        addBlock(genesisBlock);
    }

    void addBlock(Block* newBlock) {
        if (head == nullptr) {
            head = new Node(newBlock);
            tail = head;
        } else {
            Node* temp = new Node(newBlock);
            tail->next = temp;
            tail = temp;
        }
    }

    void createNewBlock(const string& dataLista) {
        int newIndex = (tail == nullptr) ? 1 : tail->block->index + 1; // Iniciar desde 1
        string prevHash = (tail == nullptr) ? "0000" : tail->block->hash;

        // Escapamos las comillas en el contenido para evitar problemas en el JSON
        string data = dataLista;

        Block* newBlock = new Block(newIndex, data, prevHash);
        newBlock->mineBlock(difficulty);
        newBlock->saveToJSON();  // Guardar el bloque en un archivo JSON
        addBlock(newBlock);
    }

    ~Blockchain() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current->block;
            delete current;
            current = next;
        }
    }
};

#endif // BLOCKCHAIN_H
