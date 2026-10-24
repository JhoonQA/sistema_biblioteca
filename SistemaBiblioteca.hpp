#ifndef SISTEMABIBLIOTECA_HPP
#define SISTEMABIBLIOTECA_HPP

#include "GestorIndice.hpp"
#include "Inicializador.hpp"

using namespace std;

class SistemaBiblioteca {
private:
    GestorIndice gestor;

    void mostrarLibro(long posicionByte) {
        if(posicionByte == -1) {
            cout<<" Libro no registrado o fue eliminado.\n";
            return;
        }
        ifstream archivo(ARCHIVO_DB, ios::binary);
        if(!archivo) return;
        archivo.seekg(posicionByte, ios::beg);
        Libro libro;
        archivo.read(reinterpret_cast<char*>(&libro), sizeof(Libro));
        archivo.close();

        cout<<"\n\tDATOS DEL LIBRO\n"
            <<"ISBN:  "<<libro.isbn<<"\n"
            <<"Title: "<<libro.titulo<<"\n"
            <<"Autor: "<<libro.autor<<"\n"
            <<"Stock: "<<libro.stock<<"\n";
    }

public:
    SistemaBiblioteca() {
        Inicializador::inicializarBaseDatos();
        gestor.cargarIndices();
        gestor.ordenar(0, gestor.obtenerTotal() - 1);
    }

    void menuInsertar() {
        Libro l;
        string isbn, titulo, autor;
        cout<<"\n\tREGISTRAR NUEVO LIBRO\nISBN (13 digitos): ";
        cin>>isbn;
        cin.ignore();

        if(gestor.buscarISBN(isbn.c_str()) != -1) {
            cout<<"El ISBN ya existe en el sistema.\n";
            return;
        }

        cout<<"Titulo: "; getline(cin, titulo);
        cout<<"Autor: "; getline(cin, autor);
        cout<<"Stock Inicial: "; cin>>l.stock;

        memset(l.isbn, 0, TAM_ISBN);
        memset(l.titulo, 0, TAM_TITULO);
        memset(l.autor, 0, TAM_AUTOR);

        memcpy(l.isbn, isbn.c_str(), (isbn.length() < TAM_ISBN - 1) ? isbn.length() : TAM_ISBN - 1);
        memcpy(l.titulo, titulo.c_str(), (titulo.length() < TAM_TITULO - 1) ? titulo.length() : TAM_TITULO - 1);
        memcpy(l.autor, autor.c_str(), (autor.length() < TAM_AUTOR - 1) ? autor.length() : TAM_AUTOR - 1);
        l.eliminado = 'F';

        if(gestor.insertarNuevoLibro(l)) cout<<"Libro guardado en disco y RAM.\n";
        else cout<<"No se pudo escribir en el archivo.\n";
    }

    void menuBuscar() {
        string isbn;
        cout<<"\n\tBUSQUEDA INDEXADA (O(log n))\nISBN a buscar: ";
        cin>>isbn;
        long pos = gestor.buscarISBN(isbn.c_str());
        mostrarLibro(pos);
    }

    void menuModificarStock(bool esPrestamo) {
        string isbn;
        cout<<"\n\tMODIFICAR STOCK\nISBN del libro: ";
        cin>>isbn;
        long pos = gestor.buscarISBN(isbn.c_str());

        if(pos == -1) {
            cout<<"Libro no encontrado!...\n";
            return;
        }

        fstream archivo(ARCHIVO_DB, ios::binary | ios::in | ios::out);
        archivo.seekg(pos, ios::beg);
        Libro libro;
        archivo.read(reinterpret_cast<char*>(&libro), sizeof(Libro));

        if(esPrestamo) {
            if(libro.stock > 0) {
                libro.stock--;
                cout<<"Prestamo procesado. Nuevo stock: "<<libro.stock<<"\n";
            } else {
                cout<<"Stock agotado.\n";
                archivo.close();
                return;
            }
        } else {
            libro.stock++;
            cout<<"Devolucion procesada. Nuevo stock: "<<libro.stock<<"\n";
        }

        archivo.seekp(pos, ios::beg);
        archivo.write(reinterpret_cast<const char*>(&libro), sizeof(Libro));
        archivo.close();
    }

    void menuEliminar() {
        string isbn;
        cout<<"\nBORRADO LOGICO\nISBN a eliminar: ";
        cin>>isbn;
        if(gestor.eliminarLogico(isbn.c_str())) cout<<"Registro removido.\n";
        else cout<<"No se encontro el ISBN.\n";
    }

    void menuMostrarTodo() {
        gestor.mostrarTablaIndices();
    }
};

#endif // SISTEMABIBLIOTECA_HPP
