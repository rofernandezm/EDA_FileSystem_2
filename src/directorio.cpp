/* 1111111 */ // sustituiir con los 7 dígitos de la cédula

#include "../include/linea.h"
#include "../include/fila.h"
#include "../include/archivo.h"
#include "../include/directorio.h"
#include <string.h>
#include <stdio.h>

typedef struct _list_archivo *LArchivos;

struct _list_archivo
{
    TArchivo file;
    LArchivos sig;
};

struct _rep_directorio
{
    Cadena name;
    TDirectorio padre;
    TDirectorio hijo;
    TDirectorio raiz;
    LArchivos archivos;
};

// Crea el directorio de nombre Raíz del filesystem
TDirectorio createRootDirectory()
{
    // Create a new repository
    TDirectorio root = new _rep_directorio();

    // Set the memory to the "name" pointer
    root->name = new char[strlen("RAIZ")];
    root->name = strcpy(root->name, "RAIZ");
    root->padre = nullptr;
    root->hijo = nullptr;
    root->archivos = nullptr;
    return root;
}

// retorna true si el directorio "directorio" no tiene archivos
bool isEmptyDirectory(TDirectorio directorio)
{
    return directorio->archivos == nullptr; // LArchivo archivos;
}

// pos-condicion: retorna true si el archivo de nombre "nombreArchivo existe en la lista de archivos "archivos".
// de lo contrario retorna false
bool existFileInLArchivos(LArchivos archivos, Cadena nombreArchivo)
{
    bool rtn = false;
    if (archivos != NULL)
    {
        if (strcmp(getFileName(archivos->file), nombreArchivo) == 0)
        {
            rtn = true;
        }
        else
        {
            rtn = existFileInLArchivos(archivos->sig, nombreArchivo);
        }
    }
    return rtn;
}

// retorna true si el archivo de nombre "nombreArchivo existe en el directorio "directorio"
bool existFileDirectory(TDirectorio directorio, Cadena nombreArchivo)
{
    return existFileInLArchivos(directorio->archivos, nombreArchivo);
}

// pre-condicion existe el archivo de nombre nombreArchivo en lista de archivos "archivos"
// pos-condicion: retorna un puntero al archivo de nombre "nombreArchivo"
TArchivo getFileInLArchivos(LArchivos archivos, Cadena nombreArchivo)
{
    TArchivo file = archivos->file;
    if (!(strcmp(getFileName(file), nombreArchivo) == 0))
    {
        file = getFileInLArchivos(archivos->sig, nombreArchivo);
    }
    return file;
}

// pre-condicion existe el archivo de nombre nombreArchivo en el directorio "directorio"
// pos-condicion: retorna un puntero al archivo de nombre "nombreArchivo"
TArchivo getFileDirectory(TDirectorio directorio, Cadena nombreArchivo)
{
    return getFileInLArchivos(directorio->archivos, nombreArchivo);
}

// pre-condicion: No existe en directorio un archivo de nombre "nombreArchivo"
// crea un archivo vacio con nombre nombreArchivo y permiso de lectura/escritura
void createFileInDirectory(TDirectorio &directorio, Cadena nombreArchivo)
{
    if (!existFileDirectory(directorio, nombreArchivo))
    {
        // Split name and ext
        // NOMBRE + SEPARADOR + EXT -> Create a copy of "nombreArchivo" to use strtok
        Cadena param = new char[strlen(nombreArchivo)];
        param = strcpy(param, nombreArchivo);
        Cadena name = strtok(param, ".");
        Cadena ext = name != NULL ? strtok(NULL, " ") : NULL;

        // NewFile
        LArchivos newFile = new _list_archivo();
        newFile->file = createEmptyFile(name, ext);
        newFile->sig = directorio->archivos;
        directorio->archivos = newFile;

        // Cleaning used pointers
        newFile = NULL;
        param = NULL;
        name = NULL;
        ext = NULL;
    }
}

/**
 * Devuelve la fila indicada por numero de "index"
 * @param archivo != NULL && cantRows(archivo) >= index
 * @return
 */
TFila getRowByIndex(TArchivo archivo, int index)
{
    TFila currentRow = firstRowFile(archivo);
    int acc = 1;
    while (acc != index)
    {
        currentRow = nextRow(currentRow);
        acc++;
    }
    return currentRow;
}

// pre-condition: archivo != NULL
TFila getLastRow(TArchivo archivo)
{
    TFila rtn = firstRowFile(archivo);
    while (!isEmptyRow(nextRow(rtn)))
    {
        TLinea curr = nextLine(headRow(rtn));
        while (curr != nullptr)
        {
            curr = nextLine(curr);
        }
        rtn = nextRow(rtn);
    }
    return rtn;
}

// pre condicion: el archivo nombreArchivo existe en directorio
// pos-condicion: inserta una nueva fila al comienzo del archivo nombreArchivo conteniendo los chars texto
void insertTextFile(TDirectorio &directorio, Cadena nombreArchivo, Cadena texto)
{
    TArchivo archivo = getFileDirectory(directorio, nombreArchivo);

    insertChartsNewRow(archivo, texto);
    archivo = nullptr;
}

// pre condicion: el archivo nombreArchivo existe en directorio
// pos-condicion: agrega al comienzo de la primera fila del archivo de nombre nombreArchivo los caracteres de texto
// desplazando los caracteres existentes hacia la derecha
void insertCharsFileFirstLine(TDirectorio &directorio, Cadena nombreArchivo, Cadena texto)
{
    TArchivo archivo = getFileDirectory(directorio, nombreArchivo); // Tengo el archivo
    TFila fila = firstRowFile(archivo);
    modifyRow(fila, texto);
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: elimina el archivo del directorio "directorio" y toda la memoria utilizada por este.
void deleteFileInLArchivos(LArchivos &archivos, Cadena nombreArchivo)
{
    if (!strcmp(getFileName(archivos->file), nombreArchivo) == 0)
    {
        deleteFileInLArchivos(archivos->sig, nombreArchivo);
    }
    else
    {
        LArchivos aux = archivos;
        archivos = archivos->sig;
        destroyFile(aux->file);
        delete aux;
    }
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: elimina el archivo del directorio "directorio" y toda la memoria utilizada por este.
void deleteFileDirectory(TDirectorio &directorio, Cadena nombreArchivo)
{
    deleteFileInLArchivos(directorio->archivos, nombreArchivo);
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: elimina los "cantidad" caracteres iniciales del archivo nombreArchivo
void deleteCharsFile(TDirectorio &directorio, Cadena nombreArchivo, int cantidad)
{
    TArchivo archivo = getFileDirectory(directorio, nombreArchivo);
    if (!isEmptyRow(firstRowFile(archivo)))
    {
        deleteCharterFile(archivo, cantidad);
    }
    archivo = nullptr;
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: le setea el permiso de escritura al archivo de nombre nombreArchivo
void setFilePermission(TDirectorio &directorio, Cadena nombreArchivo, bool permisoEscritura)
{
    TArchivo file = getFileDirectory(directorio, nombreArchivo);
    setWritePermission(file, permisoEscritura);
    file = nullptr;
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: imprime el contenido del archivo "nombreArchivo"
void printFile(TDirectorio &directorio, Cadena nombreArchivo)
{
    TArchivo file = getFileDirectory(directorio, nombreArchivo);
    int rows = getCountRow(file);
    for (int ind = 1; ind <= rows; ind++)
    {
        printLineFile(file, ind);
        printf("\n");
    }
}

// pos-condicion: destruye toda la memoria de LArchivos "archivos"
void destroyTArchivos(LArchivos &archivos)
{
    if (archivos != NULL)
    {
        if (archivos->sig != NULL)
        {
            destroyTArchivos(archivos->sig);
        }
        destroyFile(archivos->file);
        delete archivos;
    }
}

// pos-condicion destruye toda la memoria de directorio
void destroyDirectory(TDirectorio &directorio)
{
    if (directorio != NULL)
    {
        if (directorio->hijo == NULL)
        {
            destroyTArchivos(directorio->archivos);
        }
        else
        {
            destroyDirectory(directorio->hijo);
        }
    }
}