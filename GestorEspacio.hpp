#ifndef GESTORESPACIO_HPP
#define GESTORESPACIO_HPP

#include "Estructuras.hpp"
#include <stack>

using namespace std;

class GestorEspacio {
private:
    stack<long> pilaEspacios;
    const char* ARCHIVO_HUECOS = "espacios_libres.txt";

public:
    GestorEspacio() { cargarPila(); }
    ~GestorEspacio() { guardarPila(); }

    void registrarEspacio(long offset) {
        pilaEspacios.push(offset);
    }

    bool tieneEspacioLibre() const {
        return !pilaEspacios.empty();
    }

    long obtenerEspacioDisponible() {
        if(pilaEspacios.empty()) return -1;
        long offset = pilaEspacios.top();
        pilaEspacios.pop();
        return offset;
    }

    void cargarPila() {
        while(!pilaEspacios.empty()) pilaEspacios.pop();
        ifstream archivo(ARCHIVO_HUECOS);
        if(!archivo) return;
        long offset;
        while(archivo >> offset) {
            pilaEspacios.push(offset);
        }
        archivo.close();
    }

    void guardarPila() {
        ofstream archivo(ARCHIVO_HUECOS, ios::trunc);
        if(!archivo) return;
        stack<long> copia = pilaEspacios;
        while(!copia.empty()) {
            archivo << copia.top() << "\n";
            copia.pop();
        }
        archivo.close();
    }

    int obtenerTotalHuecos() const { return pilaEspacios.size(); }
};

#endif
