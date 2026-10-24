# Sistema de Biblioteca Indexada en C++

Sistema de gestión de biblioteca de alto rendimiento desarrollado en C++. 

## Requisitos

- Compilador con soporte para C++17 o superior (como `g++`, `clang++` o MSVC).
- Herramienta de construcción `make`.

## Compilación e Instalación

Para compilar la aplicación principal utilizando `g++`:

```bash
g++ -std=c++17 main.cpp -o sistema_biblioteca
```

Para compilar el ejecutable de pruebas de rendimiento y benchmark de tiempos:

```bash
g++ -std=c++17 main_tiempo_pruebas.cpp -o pruebas_tiempo
```

## Ejecución

### Aplicación Principal
Ejecuta el sistema interactivo de la biblioteca:

```bash
./sistema_biblioteca
```

### Pruebas de Rendimiento
Ejecuta el benchmark de mediciones de tiempo e inserción masiva utilizando las semillas de prueba:

```bash
./pruebas_tiempo
```

## Observaciones

- El sistema está diseñado sin depender de bases de datos externas; toda la lógica de persistencia, direccionamiento binario (*offsets*) e indexación se implementa directamente en C++.
- La gestión de memoria y archivos sigue el paradigma RAII para garantizar la liberación segura de recursos de hardware y descriptores de archivo.