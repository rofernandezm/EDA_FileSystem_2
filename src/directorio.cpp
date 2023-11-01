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
    TDirectorio father;       // padre
    TDirectorio firstSibling; // primerHijo
    TDirectorio nextBrother;  // siguiente hermano
    TDirectorio raiz;
    LArchivos archivos;
    TDirectorio currentDirectory;
};

// Crea el directorio de nombre Raíz del filesystem
TDirectorio createRootDirectory()
{
    // Create a new repository
    TDirectorio root = new _rep_directorio();

    // Set the memory to the "name" pointer
    root->name = new char[strlen("RAIZ")];
    root->name = strcpy(root->name, "RAIZ");
    root->father = NULL;
    root->firstSibling = NULL;
    root->archivos = NULL;
    root->nextBrother = NULL;
    root->currentDirectory = root;
    return root;
}

// retorna true si el directorio "directorio" no tiene archivos
bool isEmptyDirectory(TDirectorio directorio)
{
    bool rtn = directorio->archivos == NULL && directorio->firstSibling == NULL && directorio->nextBrother == NULL;
    return rtn; // LArchivo archivos;
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
        while (curr != NULL)
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
    archivo = NULL;
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
    archivo = NULL;
}

// pre-condicion: existe el archivo de nombre "nombreArchivo" en el directorio "directorio"
// pos-condicion: le setea el permiso de escritura al archivo de nombre nombreArchivo
void setFilePermission(TDirectorio &directorio, Cadena nombreArchivo, bool permisoEscritura)
{
    TArchivo file = getFileDirectory(directorio, nombreArchivo);
    setWritePermission(file, permisoEscritura);
    file = NULL;
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
        if (directorio->firstSibling == NULL)
        {
            destroyTArchivos(directorio->archivos);
        }
        else
        {
            destroyDirectory(directorio->firstSibling);
        }
    }
}

//******************************Nuevas funciones *****************************************************************

// retorna true si el directorio de nombre nombreDierctorioHijo es hijo del directorio "directorio"
bool existChildrenDirectory(TDirectorio directorio, Cadena nombreDirectorioHijo)
{

    bool rtn = false;
    TDirectorio hijo = directorio->firstSibling;

    while (hijo != NULL && strcmp(hijo->name, nombreDirectorioHijo) != 0)
    {
        hijo = hijo->nextBrother;
    }

    if (hijo != NULL)
    {
        rtn = true;
    }

    return rtn;
}

// pre-condición el directorio de nombre nombreDirectorioHijo es hijo del directorio Directorio
// pos-condición retorna un puntero al directorio de nombre nombreDirectorioHijo
TDirectorio moveChildrenDirectory(TDirectorio &directorio, Cadena nombreDirectorioHijo)
{

    TDirectorio hijo = directorio->firstSibling;
    while (strcmp(hijo->name, nombreDirectorioHijo) != 0)
    {
        hijo = hijo->nextBrother;
    }
    directorio->currentDirectory = hijo;
    return hijo;
}

// retorna un puntero de TDirectorio al padre del directorio directorio
TDirectorio moveFatherDirectory(TDirectorio &directorio)
{
    return directorio->father;
}

// retorna un puntero de TDirectorio al directorio ROOT
TDirectorio moveRootDirectory(TDirectorio &directorio)
{
    return directorio->raiz;
}

// retorna true si el directorio directorio es root
bool isRootDirectory(TDirectorio directorio)
{
    bool rtn = false;

    if (strcmp(directorio->name, "RAIZ") == 0)
    {
        rtn = true;
    }

    return rtn;
}

// Pre-Condición del directorio de nombre nombreDirectorio no es hijo del directorio "directorio"
// pos-condición crea un directorio vacío, de nombre nombreDirectorio, hijo del directorio "directorio"
void createChildrenDirectory(TDirectorio &directorio, Cadena nombreDirectorio)
{
    Cadena name = new char[strlen(nombreDirectorio) + 1];
    name = strcpy(name, nombreDirectorio);

    TDirectorio newDir = new _rep_directorio;
    newDir->name = name;
    newDir->firstSibling = NULL;
    newDir->father = directorio;
    newDir->nextBrother = NULL;

    TDirectorio iter = directorio->firstSibling;
    if (isEmptyDirectory(directorio) || strcmp(nombreDirectorio, iter->name) < 0)
    {
        newDir->nextBrother = iter;
        directorio->firstSibling = newDir;
        // printf("directorio.cpp::createChildrenDirectory::Inserto al inicio del dir::%s\n", directorio->currentDirectory->name);
    }
    else
    {
        bool notInserted = true;
        while (iter->nextBrother != NULL && notInserted)
        {
            if (strcmp(nombreDirectorio, iter->nextBrother->name) < 0)
            {
                newDir->nextBrother = iter->nextBrother;
                iter->nextBrother = newDir;
                notInserted = false;
                // printf("directorio.cpp::createChildrenDirectory::Se insertó en la mitad del directorio::%s\n", directorio->currentDirectory->name);
            }
            iter = iter->nextBrother;
        }

        if (notInserted && iter->nextBrother == NULL)
        {
            newDir->nextBrother = iter->nextBrother;
            iter->nextBrother = newDir;
            // printf("directorio.cpp::createChildrenDirectory::Se insertó en el final del directorio::%s\n", directorio->currentDirectory->name);
        }
    }

    name = NULL;
    newDir = NULL;
    iter = NULL;
}

// pre-condición el directorio de nombre nombreDirectorio es hijo del directorio directorio
// pos-condición elimina el directorio de nombre nombreDirectorio que es hijo del directorio directorio
// eliminando toda su memoria
void removeChildrenDirectory(TDirectorio &directorio, Cadena nombreDirectorio);

// pre-condición el directorio origen es sub-directorio del directorio "directorio"
// pos-condición mueve el directorio origen y todo su contenido al directorio destino
void moveSubDirectory(TDirectorio &directorio, TDirectorio origen, TDirectorio &destino);

// pre-condición el archivo origen es sub archivo del directorio directorio
// pos-condición se mueve el archivo TArchivo como hijo del directorio destino
void moveSubArchive(TDirectorio &directorio, TArchivo origen, TDirectorio destino);

// pre-condición: directorio no es el directorio ROOT
// pos-condición: retorna un puntero al primer hermano del diretorio "directorio"
TDirectorio firstBrotherDirectory(TDirectorio directorio);

// pos-condición: retorna un puntero al primer hijo del directorio "directorio"
TDirectorio firstChildrenDirectory(TDirectorio directorio);

// Retorna true si el directorio subdir es sub-directorio del directorio "directorio" en cualquier nivel.
bool isSubDirectoryRoot(TDirectorio directorio, Cadena ruta);

// pos-condición imprime el directorio ejecuando DIR
void printDirectoryDir(TDirectorio directorio)
{
    printf("%s\n", directorio->name);
    if (!isEmptyDirectory(directorio))
    {
        TDirectorio iter = directorio->firstSibling;
        while (iter != NULL)
        {
            printf("%s/%s\n", directorio->name, iter->name);
            iter = iter->nextBrother;
        }
    }
    else
    {
        printf("No existen archivos ni directorios.\n");
    }
}

//Auxiliar para imprimir archivos del directorio "directorio" con el formato directorio->name/archivo->name
void printFilesNameInCurrentDirectory(TDirectorio directorio)
{
    printf("%s\n", directorio->name);
    if (!isEmptyDirectory(directorio))
    {
        LArchivos files = directorio->archivos;

        while (files != NULL)
        {
            printf("%s/%s\n", directorio->name, getFileName(files->file));
            files = files->sig;
        }
    }
}

// pos-condición imprime el directorio ejecutando DIR /S
void printDirectoryDirS(TDirectorio directorio)
{
    if (!isEmptyDirectory(directorio->nextBrother))
    {

        printf("%s \n", directorio->name);
        // imprimir los archivos del directorio actual
        printDirectoryDirS(directorio->firstSibling);
        printDirectoryDirS(directorio->nextBrother);
    }
}