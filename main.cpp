#include "SistemaBiblioteca.hpp"

using namespace std;

int main() {
    SistemaBiblioteca biblioteca;
    int opcion;

    do {
            cout<<"\n\tSISTEMA DE BIBLIOTECA INDEXADO \n"
            <<"[1] Buscar Libro\n"
            <<"[2] Registrar Nuevo Libro\n"
            <<"[3] Prestar Libro\n"
            <<"[4] Devolver Libro\n"
            <<"[5] Eliminar Libro\n"
            <<"[6] Ver Tabla de Indices en RAM\n"
            <<"[7] Salir\n"
            <<"Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch(opcion) {
            case 1: biblioteca.menuBuscar(); break;
            case 2: biblioteca.menuInsertar(); break;
            case 3: biblioteca.menuModificarStock(true); break;
            case 4: biblioteca.menuModificarStock(false); break;
            case 5: biblioteca.menuEliminar(); break;
            case 6: biblioteca.menuMostrarTodo(); break;
            case 7: cout<<"\nCerrando...\n"; break;
            default: cout<<"Opcion invalida.\n";
        }
    } while(opcion != 7);

    return 0;
}
