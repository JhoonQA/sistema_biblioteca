
#ifndef INDICESECUNDARIO_HPP
#define INDICESECUNDARIO_HPP

#include "Estructuras.hpp"
#include <unordered_map>

using namespace std;

class IndiceSecundario {
private:
    unordered_map<string, vector<long>> tablaAutor;

public:
    void mapearRegistro(const string& autor, long offset) {
        tablaAutor[autor].push_back(offset);
    }

    void desmapearRegistro(const string& autor, long offset) {
        if(tablaAutor.find(autor) == tablaAutor.end()) return;
        vector<long>& lista = tablaAutor[autor];
        for(auto it = lista.begin(); it != lista.end(); ++it) {
            if(*it == offset) {
                lista.erase(it);
                break;
            }
        }
        if(lista.empty()) tablaAutor.erase(autor);
    }

    vector<long> buscarPorAutor(const string& autor) {
        if(tablaAutor.find(autor) == tablaAutor.end()) return vector<long>();
        return tablaAutor[autor];
    }

    void limpiar() { tablaAutor.clear(); }
    int obtenerTotalAutores() const { return tablaAutor.size(); }
};

#endif
