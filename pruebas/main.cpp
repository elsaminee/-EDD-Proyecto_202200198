#include "Matrix.h" // Asegúrate de que el archivo Matrix.h esté en la misma carpeta o ajusta el path

int main() {
    Matrix matrizAmistad;

    // Insertar solicitudes de amistad
    matrizAmistad.insert("Alice", "Bob");
    matrizAmistad.insert("Alice", "Charlie");
    matrizAmistad.insert("Alice", "David");
    matrizAmistad.insert("Bob", "Alice");
    matrizAmistad.insert("Bob", "Charlie");
    matrizAmistad.insert("Charlie", "Alice");

    matrizAmistad.print();
    matrizAmistad.startFriendIteration("Alice");
    string friendName;
    while (!(friendName = matrizAmistad.getNextFriend()).empty()) {
        cout << "Alice is friends with: " << friendName << endl;
    }
    return 0;
}