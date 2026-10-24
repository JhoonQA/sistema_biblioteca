
#ifndef ESTRUCTURAS_HPP
#define ESTRUCTURAS_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>

const int TAM_ISBN = 14;   // 13 dígitos + '\0'
const int TAM_TITULO = 60; // Limite maximo para el titulo
const int TAM_AUTOR = 40;  // Limite maximo para el autor

const char ARCHIVO_DB[] = "libros.dat";
const char ARCHIVO_SEMILLAS[] = "semillas.txt";

struct Libro {
    char isbn[TAM_ISBN];
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    int stock;
    char eliminado;
};

struct IndiceLibro {
    char isbn[TAM_ISBN];
    long posicion_bytes;
};

#endif
