#include <iostream>
#include <fstream>
#include <cstdlib>
#include <locale>
#include <windows.h>

using namespace std;

void administrador();
void user();

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

void administrador() {
    cout << "Menú de administrador" << endl;
    cout << " ----------------------" << endl;
    cout << " 1. Carga de usuarios" << endl;
    cout << " 2. Carga de relaciones" << endl;
    cout << " 3. Carga de publicaciones" << endl;
    cout << " 4. Gestionar usuarios" << endl;
    cout << " 5. Reportes" << endl;
    cout << " 6. Salir" << endl;
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