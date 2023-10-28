/* 1111111 */ // sustituiir con los 7 dígitos de la cédula

#include "../include/archivo.h"
#include "../include/linea.h"
#include "../include/fila.h"
#include <string.h>
#include <stdio.h>

struct _rep_archivo
{
    Cadena name;
    Cadena ext;
    bool write;
    TFila fila;
};

// Crea un archivo vacío con nombre nombreArchivo, extensión extension y con permiso de lectura/escritura
// El archivo no contiene filas.
TArchivo createEmptyFile(Cadena nombreArchivo, Cadena extension)
{
    // Se crea nueva instancia de TArchivo;
    TArchivo file = new _rep_archivo();
    setName(file, nombreArchivo);
    setExtension(file, extension);
    setWritePermission(file, true);
    file->fila = createRow();
    return file;
}

// Retorna un puntero a un array con el nombre del archivo "archivo"
char *getFileName(TArchivo archivo)
{
    int length = strlen(archivo->name) + strlen(archivo->ext) + 1;
    Cadena name = new char[length];
    name = strcpy(name, archivo->name);
    strcat(name, ".");
    strcat(name, archivo->ext);
    return name;
}

// Retorna true si archivo tiene permiso de escritura
bool haveWritePermission(TArchivo archivo)
{
    return archivo->write;
}

// retorna true si archivo no tiene filas;
bool isEmptyFile(TArchivo archivo)
{
    return archivo->fila == nullptr;
}

// Retorna un puntero a la primer Fila de archivo
TLinea getFirstLine(TArchivo archivo)
{
    TLinea rtn = nullptr;
    if (archivo->fila != nullptr)
    {
        TFila firstRow = firstRowFile(archivo);
        rtn = headRow(firstRow);
        firstRow = nullptr;
    }
    return rtn;
}

// Retorna un puntero a la siguiente Fila de archivo
TFila getNextRow(TArchivo archivo)
{
    TFila rtn;
    if (archivo->fila != nullptr)
    {
        rtn = nextRow(firstRowFile(archivo));
    }
    return rtn;
}

// Retorna la cantidad de Fila que tiene el archivo "archivo"
int getCountRow(TArchivo archivo)
{
    int rtn = 0;
    TFila fila = firstRowFile(archivo);

    while (!isEmptyRow(fila))
    {
        fila = nextRow(fila);
        rtn++;
    }
    fila = nullptr;
    return rtn;
}

// Retorna la cantidad de caracteres que tiene el archivo "archivo"
int getCountChars(TArchivo archivo)
{
    int rtn = 0;
    int rows = getCountRow(archivo);
    if (rows > 0)
    {
        TFila currentRow = archivo->fila; // changed
        while (rows > 0)
        {
            rtn += countNodesLine(headRow(currentRow)); // Changed
            currentRow = nextRow(currentRow);
            rows--;
        }
    }
    return rtn;
}

// imprime la Linea del archivo indicada por "numero_linea"
// pre-condición el archivo tiene por lo menos numero_linea de lineas
void printLineFile(TArchivo archivo, int numero_linea)
{
    // cantidad de filas
    int rows = getCountRow(archivo);
    TFila currentRow = archivo->fila;
    int cont = 1;
    while (cont != numero_linea && cont <= rows)
    {
        currentRow = nextRow(currentRow);
        cont++;
    }

    TLinea currentLine = nextLine(headRow(currentRow));
    while (currentLine != nullptr)
    {
        printf("%c", firstCharLine(currentLine));
        currentLine = nextLine(currentLine);
    }
}

// Elimina los cant cantidad de caracteres finales del "archivo"
// En caso que el archivo tenga menos caracteres los elimina a todos
void deleteCharterFile(TArchivo &archivo, int cant)
{
    int countChars = getCountChars(archivo);
    if (cant >= countChars)
    {
        TFila auxFirstRow = firstRowFile(archivo);
        archivo->fila = nullptr;
        deleteRows(auxFirstRow);
    }
    else
    {
        int diff = cant;
        while (diff > 0)
        {
            TFila lastRow = archivo->fila;
            while (!isEmptyRow(nextRow(lastRow)))
            {
                lastRow = nextRow(lastRow);
            }
            TLinea currentLine = headRow(lastRow);
            int nodesInLine = countNodesLine(currentLine);
            if (diff >= nodesInLine)
            {
                // ELIMINAR LINEA Y FILA
                diff -= nodesInLine;
                deleteLastRow(archivo->fila);
            }
            else
            {
                // ELIMINAR N NODOS
                while (diff > 0)
                {
                    deleteLastChar(currentLine);
                    diff--;
                }
            }
            currentLine = nullptr;
            lastRow = nullptr;
        }
    }
}

// Cambia el nombre del archivo "archivo" por nuevoNombre
void setName(TArchivo &archivo, Cadena nuevoNombre)
{
    if (archivo->name == nullptr)
    {
        archivo->name = new char[strlen(nuevoNombre)];
    }
    archivo->name = strcpy(archivo->name, nuevoNombre);
}

// Cambia la extension del "archivo" por nuevaExtension
void setExtension(TArchivo &archivo, Cadena nuevaExtension)
{
    if (archivo->ext == nullptr)
    {
        archivo->ext = new char[strlen(nuevaExtension)];
    }
    archivo->ext = strcpy(archivo->ext, nuevaExtension);
}

// pre-condicion El archivo tiene por lo menos una fila
// Inserta el texto "texto" al inicio de la primer fila del archivo
void insertChartsFirstRow(TArchivo &archivo, Cadena texto)
{
    Cadena textCopy = new char[strlen(texto)];
    textCopy = strcpy(textCopy, texto);

    TLinea auxL = getFirstLine(archivo); // siguiente de dummy
    int largo = strlen(textCopy) - 1;

    while (largo > 0)
    {
        insertCharLine(textCopy[largo], auxL);
        largo--;
    }

    auxL = nullptr;
    textCopy = nullptr;
}

// si valor == true se le asigna el permiso de escritura de "archivo"
// si valor == false se le quita el permiso de escritura de "archivo"
// pre-condicion archivo !=NULL
void setWritePermission(TArchivo &archivo, bool valor)
{
    archivo->write = valor;
}

// elimina toda la memoria utilizada por "archivo"
void destroyFile(TArchivo &archivo)
{
    if (archivo != nullptr)
    {
        TArchivo aux = archivo;
        deleteRows(aux->fila);
        delete aux;
        archivo = nullptr;
    }
}

// Retorna retorna un puntero a la primer fila del archivo "archivo"
TFila firstRowFile(TArchivo archivo)
{
    return archivo->fila;
}

// Inserta el texto "texto" como una nueva fila al comienzo del archivo
void insertChartsNewRow(TArchivo &archivo, Cadena texto)
{

    Cadena textCopy = new char[strlen(texto)];
    textCopy = strcpy(textCopy, texto);

    TLinea newLine = createLine();
    insertRow(archivo->fila, newLine);

    TLinea auxL = getFirstLine(archivo); // siguiente de dummy
    int largo = strlen(textCopy) - 1;

    while (largo > 0)
    {
        // VER NUEVO NODO
        insertCharLine(textCopy[largo], auxL);
        largo--;
    }

    newLine = nullptr;
    auxL = nullptr;
}
