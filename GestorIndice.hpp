#ifndef GESTORINDICE_HPP
#define GESTORINDICE_HPP

#include "Estructuras.hpp"
#include "GestorEspacio.hpp"
#include "IndiceSecundario.hpp"

using namespace std;

class GestorIndice {
private:
    vector<IndiceLibro> indices;
    GestorEspacio gestorEspacio;
    IndiceSecundario indiceAutor;

    void mergeSort(int inicio, int mitad, int fin) {
        int n1=mitad-inicio+1;
        int n2=fin-mitad;
        vector<IndiceLibro> izq(n1);
        vector<IndiceLibro> der(n2);
        for(int i=0;i<n1;++i) izq[i]=indices[inicio+i];
        for(int j=0;j<n2;++j) der[j]=indices[mitad+1+j];
        int i=0,j=0,k=inicio;
        while(i<n1 && j<n2) {
            if(strcmp(izq[i].isbn,der[j].isbn)<=0) {
                indices[k]=izq[i];
                ++i;
            } else {
                indices[k]=der[j];
                ++j;
            }
            ++k;
        }
        while(i<n1) { indices[k]=izq[i]; ++i; ++k; }
        while(j<n2) { indices[k]=der[j]; ++j; ++k; }
    }

public:
    bool cargarIndices() {
        ifstream archivo(ARCHIVO_DB,ios::binary);
        if(!archivo) return false;
        indices.clear();
        indiceAutor.limpiar();
        Libro libro;
        long posicionByte=0;
        while(archivo.read(reinterpret_cast<char*>(&libro),sizeof(Libro))) {
            if(libro.eliminado=='F') {
                IndiceLibro nuevoIndice;
                strcpy(nuevoIndice.isbn,libro.isbn);
                nuevoIndice.posicion_bytes=posicionByte;
                indices.push_back(nuevoIndice);

                indiceAutor.mapearRegistro(libro.autor, posicionByte);
            }else if(libro.eliminado == 'V'){
                gestorEspacio.registrarEspacio(posicionByte);
            }
            posicionByte=archivo.tellg();
        }
        archivo.close();
        return true;
    }

    void ordenar(int inicio, int fin) {
        if(inicio>=fin) return;
        int mitad=inicio+(fin-inicio)/2;
        ordenar(inicio,mitad);
        ordenar(mitad+1,fin);
        mergeSort(inicio,mitad,fin);
    }

    long buscarISBN(const char* claveIsbn) {
        int izq=0;
        int der=indices.size()-1;
        while(izq<=der) {
            int mitad=izq+(der-izq)/2;
            int comparacion=strcmp(indices[mitad].isbn,claveIsbn);
            if(comparacion==0) return indices[mitad].posicion_bytes;
            if(comparacion<0) izq=mitad+1;
            else der=mitad-1;
        }
        return -1;
    }

    vector<long> buscarPorAutor(const string& autor) {
        return indiceAutor.buscarPorAutor(autor);
    }

    bool eliminarLogico(const char* claveIsbn) {
        int izq=0;
        int der=indices.size()-1;
        int posicionIndiceRAM=-1;
        long posicionByteDisco=-1;

        // Busqueda binaria
        while(izq<=der) {
            int mitad=izq+(der-izq)/2;
            int comparacion=strcmp(indices[mitad].isbn,claveIsbn);
            if(comparacion==0) {
                posicionIndiceRAM=mitad;
                posicionByteDisco=indices[mitad].posicion_bytes;
                break;
            }
            if(comparacion<0) izq=mitad+1;
            else der=mitad-1;
        }

        if(posicionIndiceRAM==-1) return false;



        fstream archivo(ARCHIVO_DB,ios::binary|ios::in|ios::out);
        if(!archivo) return false;
        archivo.seekg(posicionByteDisco, ios::beg);
        Libro libroABorrar;
        archivo.read(reinterpret_cast<char*>(&libroABorrar), sizeof(Libro));

        long offsetBandera=posicionByteDisco+offsetof(Libro,eliminado);
        archivo.seekp(offsetBandera,ios::beg);
        char bandera='V';
        archivo.write(&bandera,sizeof(char));
        archivo.close();

        gestorEspacio.registrarEspacio(posicionByteDisco);
        indiceAutor.desmapearRegistro(libroABorrar.autor, posicionByteDisco);

        indices.erase(indices.begin()+posicionIndiceRAM);
        return true;
    }
    bool insertarNuevoLibro(const Libro& nuevoLibro) {
        if(buscarISBN(nuevoLibro.isbn)!=-1) return false;

        long posicionByte = -1;

        if(gestorEspacio.tieneEspacioLibre()) {
            posicionByte = gestorEspacio.obtenerEspacioDisponible();
            fstream archivo(ARCHIVO_DB, ios::binary | ios::in | ios::out);
            if(!archivo) return false;
            archivo.seekp(posicionByte, ios::beg); // Sobreescritura directa en el hueco
            archivo.write(reinterpret_cast<const char*>(&nuevoLibro), sizeof(Libro));
            archivo.close();
        } else {
            ofstream archivo(ARCHIVO_DB, ios::binary | ios::app);
            if(!archivo) return false;
            archivo.seekp(0, ios::end);
            posicionByte = archivo.tellp();
            archivo.write(reinterpret_cast<const char*>(&nuevoLibro), sizeof(Libro));
            archivo.close();
        }

        IndiceLibro nuevoIndice;
        strcpy(nuevoIndice.isbn,nuevoLibro.isbn);
        nuevoIndice.posicion_bytes=posicionByte;

        size_t i=0;
        while(i<indices.size() && strcmp(indices[i].isbn,nuevoIndice.isbn)<0) {
            ++i;
        }
        indices.insert(indices.begin()+i,nuevoIndice);

        indiceAutor.mapearRegistro(nuevoLibro.autor, posicionByte);

        return true;
    }

    int obtenerTotal() const {
        return indices.size();
    }
    int obtenerTotalHuecos() const {
        return gestorEspacio.obtenerTotalHuecos();
    }
    int obtenerTotalAutores() const {
        return indiceAutor.obtenerTotalAutores();
    }

    void mostrarTablaIndices() const {
        cout<<"\nTABLA DE INDICES EN RAM\n";
        for(size_t i=0;i<indices.size();++i) {
            cout<<"["<<i<<"] ISBN:"<<indices[i].isbn<<" -> Byte:"<<indices[i].posicion_bytes<<"\n";
        }
    }
};

#endif
