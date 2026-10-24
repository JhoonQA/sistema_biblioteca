#ifndef INICIALIZADOR_HPP
#define INICIALIZADOR_HPP

#include "Estructuras.hpp"
#include <sstream>

using namespace std;

class Inicializador {
private:

    static void copiarCadenaSegura(char* destino, const string& origen, int tamMaximo) {
        memset(destino, 0, tamMaximo);
        int caracteresACopiar = (origen.length() < tamMaximo - 1) ? origen.length() : tamMaximo - 1;
        memcpy(destino, origen.c_str(), caracteresACopiar);
    }

public:

    static bool inicializarBaseDatos() {
        ifstream testFichero(ARCHIVO_DB, ios::binary);
        if (testFichero.good()) {
            cout<<"Base de datos '" << ARCHIVO_DB << "' detectada.\n";
            testFichero.close();
            return true;
        }
        testFichero.close();

        ifstream archivoTexto(ARCHIVO_SEMILLAS);
        if (!archivoTexto.is_open()) {
            cerr << "No se encontro el archivo '" << ARCHIVO_SEMILLAS << "' con los registros.\n";
            return false;
        }

        ofstream archivoBinario(ARCHIVO_DB, ios::binary | ios::out);
        if (!archivoBinario.is_open()) {
            cerr<<"No se pudo crear el archivo binario '" << ARCHIVO_DB << "'.\n";
            archivoTexto.close();
            return false;
        }

        string linea;
        int registrosProcesados = 0;

        while (getline(archivoTexto, linea)) {
            if (linea.empty()) continue; // Omitir líneas vacías accidentalmente

            stringstream ss(linea);
            string isbnToken, tituloToken, autorToken, stockToken;

            if (getline(ss, isbnToken, ',') &&
                getline(ss, tituloToken, ',') &&
                getline(ss, autorToken, ',') &&
                getline(ss, stockToken, ',')) {

                Libro nuevoLibro;

                copiarCadenaSegura(nuevoLibro.isbn, isbnToken, TAM_ISBN);
                copiarCadenaSegura(nuevoLibro.titulo, tituloToken, TAM_TITULO);
                copiarCadenaSegura(nuevoLibro.autor, autorToken, TAM_AUTOR);
                nuevoLibro.stock = stoi(stockToken);
                nuevoLibro.eliminado = 'F';

                archivoBinario.write(reinterpret_cast<const char*>(&nuevoLibro), sizeof(Libro));
                registrosProcesados++;
            }
        }

        archivoTexto.close();
        archivoBinario.close();
        cout<<"Carga completada. Registros insertados: " << registrosProcesados << "\n";
        return true;
    }
};

#endif // INICIALIZADOR_HPP
