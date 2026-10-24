
#include "SistemaBiblioteca.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    cout<<"\tRENDIMIENTO Y TIEMPOS\n";
    remove(ARCHIVO_DB);

    auto tStart = high_resolution_clock::now();
    bool initOk = Inicializador::inicializarBaseDatos();
    auto tEnd = high_resolution_clock::now();

    auto tParsing = duration_cast<milliseconds>(tEnd - tStart).count();

    if(!initOk) return 1;

    GestorIndice gestor;

    tStart = high_resolution_clock::now();
    gestor.cargarIndices();
    tEnd = high_resolution_clock::now();

    auto tCargaRAM = duration_cast<microseconds>(tEnd - tStart).count();

    int totalRegs = gestor.obtenerTotal();

    tStart = high_resolution_clock::now();
    gestor.ordenar(0, totalRegs - 1);
    tEnd = high_resolution_clock::now();

    auto tOrdenamiento = duration_cast<microseconds>(tEnd - tStart).count();

    Libro libroPrueba;
    strcpy(libroPrueba.isbn, "9780000000001");
    strcpy(libroPrueba.titulo, "Libro de Prueba de Rendimiento");
    strcpy(libroPrueba.autor, "Autor de Control");
    libroPrueba.stock = 5;
    libroPrueba.eliminado = 'F';

    tStart = high_resolution_clock::now();
    gestor.insertarNuevoLibro(libroPrueba);
    tEnd = high_resolution_clock::now();

    auto tInsercionUnitaria = duration_cast<microseconds>(tEnd - tStart).count();

    cout<<"\n\tRESULTADOS DEL ANALISIS\n"
        <<"- Parsing Inicial (Texto -> .dat) : "<<tParsing<<" ms\n"
        <<"- Registro Fisico de cada Libro   : ~"<<(double)tParsing/totalRegs<<" ms/reg\n"
        <<"- Carga de Indices (Disco -> RAM) : "<<tCargaRAM<<" us\n"
        <<"- Ordenamiento RAM (Merge Sort)   : "<<tOrdenamiento<<" us\n"
        <<"- Insercion Unitaria Ordenada     : "<<tInsercionUnitaria<<" us\n"
        <<"Total de registros activos en RAM : "<<gestor.obtenerTotal()<<"\n";
    return 0;
}
