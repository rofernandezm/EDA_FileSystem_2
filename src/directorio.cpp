/* 1111111 */ // sustituiir con los 7 dígitos de la cédula

#include "../include/linea.h"
#include "../include/fila.h"
#include "../include/archivo.h"
#include "../include/directorio.h"
#include <string.h>
#include <stdio.h>
#define READ_PERMISSION "Lectura"
#define WRITE_PERMISSION "Lectura/Escritura"
#define ROOT_NAME "RAIZ"
#define SLASH "/"

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
    root->name = new char[strlen(ROOT_NAME)];
    root->name = strcpy(root->name, ROOT_NAME);
    root->father = NULL;
    root->firstSibling = NULL;
    root->archivos = NULL;
    root->nextBrother = NULL;
    root->currentDirectory = root;
    root->raiz = root;
    return root;
}

// retorna true si el directorio "directorio" no tiene archivos
bool isEmptyDirectory(TDirectorio directorio)
{
    bool rtn = directorio->archivos == NULL;
    return rtn;
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

// pre-condicion: existe el archivo de nombre nombreArchivo en lista de archivos "archivos"
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

// pre-condicion: existe el archivo de nombre nombreArchivo en el directorio "directorio"
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
        newFile->sig = NULL;

        LArchivos files = directorio->archivos;
        if (files == NULL || strcmp(getFileName(newFile->file), getFileName(files->file)) < 0)
        {
            newFile->sig = directorio->archivos;
            directorio->archivos = newFile;
        }
        else
        {
            bool notInserted = true;
            while (files->sig != NULL && notInserted)
            {
                if (strcmp(getFileName(newFile->file), getFileName(files->sig->file)) < 0)
                {
                    newFile->sig = files->sig;
                    files->sig = newFile;
                    notInserted = false;
                }
                files = files->sig;
            }

            if (notInserted && files->sig == NULL)
            {
                newFile->sig = files->sig;
                files->sig = newFile;
            }
        }

        // Cleaning used pointers
        files = NULL;
        newFile = NULL;

        delete[] param;
        param = NULL;
        name = NULL;
        ext = NULL;
    }
}

// pre-condicion: el archivo nombreArchivo existe en directorio
// pos-condicion: inserta una nueva fila al comienzo del archivo nombreArchivo conteniendo los chars texto
void insertTextFile(TDirectorio &directorio, Cadena nombreArchivo, Cadena texto)
{
    TArchivo archivo = getFileDirectory(directorio, nombreArchivo);
    insertChartsNewRow(archivo, texto);
    archivo = NULL;
}

// pre-condicion: el archivo nombreArchivo existe en directorio
// pos-condicion: agrega al comienzo de la primera fila del archivo de nombre nombreArchivo los caracteres de texto
// desplazando los caracteres existentes hacia la derecha
void insertCharsFileFirstLine(TDirectorio &directorio, Cadena nombreArchivo, Cadena texto)
{
    TArchivo archivo = getFileDirectory(directorio, nombreArchivo); // Tengo el archivo
    TFila fila = firstRowFile(archivo);
    modifyRow(fila, texto);

    archivo = NULL;
    fila = NULL;
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
    file = NULL;
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
    }
}

// pos-condicion: destruye toda la memoria de directorio
void destroyChildrenDirectory(TDirectorio &directorio)
{
    if (directorio != NULL)
    {
        if (!isEmptyDirectory(directorio))
        {
            destroyTArchivos(directorio->archivos);
        }
        destroyChildrenDirectory(directorio->firstSibling);
        destroyChildrenDirectory(directorio->nextBrother);
        delete directorio;
    }
}

// pos-condicion: destruye toda la memoria de directorio
void destroyDirectory(TDirectorio &directorio)
{
    if (directorio != NULL)
    {
        if (!isEmptyDirectory(directorio))
        {
            destroyTArchivos(directorio->archivos);
        }
        destroyChildrenDirectory(directorio->firstSibling);
        delete directorio;
    }
}

// pre-condicion: directorio != NULL
// pos-condicion: retorna el nombre de directorio
Cadena getDirectoryName(TDirectorio directorio)
{
    return directorio->name;
}

// pre-condicion: directorio != NULL
// pos-condicion: retorna true si el directorio de nombre nombreDierctorioHijo es hijo del directorio "directorio"
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

// pre-condicion: el directorio de nombre nombreDirectorioHijo es hijo del directorio Directorio
// pos-condicion: retorna un puntero al directorio de nombre nombreDirectorioHijo
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
    if (strcmp(directorio->name, ROOT_NAME) == 0)
    {
        rtn = true;
    }

    return rtn;
}

// pre-Condición del directorio de nombre nombreDirectorio no es hijo del directorio "directorio"
// pos-condicion crea un directorio vacío, de nombre nombreDirectorio, hijo del directorio "directorio"
void createChildrenDirectory(TDirectorio &directorio, Cadena nombreDirectorio)
{
    Cadena name = new char[strlen(nombreDirectorio) + 1];
    name = strcpy(name, nombreDirectorio);

    TDirectorio newDir = new _rep_directorio;
    newDir->name = name;
    newDir->firstSibling = NULL;
    newDir->father = directorio;
    newDir->nextBrother = NULL;
    newDir->raiz = directorio->raiz;

    TDirectorio iter = directorio->firstSibling;
    if (iter == NULL || strcmp(nombreDirectorio, iter->name) < 0)
    {
        newDir->nextBrother = iter;
        directorio->firstSibling = newDir;
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
            }
            iter = iter->nextBrother;
        }

        if (notInserted && iter->nextBrother == NULL)
        {
            newDir->nextBrother = iter->nextBrother;
            iter->nextBrother = newDir;
        }
    }

    name = NULL;
    newDir = NULL;
    iter = NULL;
}

// pre-condicion: Existe el subdirectorio "nombreDirectorio" en directorio y "nombreDirectorio" no es el primer hijo de directorio
// pos-condicion: Devuelve el nodo anterior al subdirectorio "nombreDirectorio"
TDirectorio getPreviousBrother(TDirectorio directorio, Cadena nombreDirectorio)
{
    TDirectorio rtn = directorio->firstSibling;
    if (!strcmp(rtn->nextBrother->name, nombreDirectorio) == 0)
    {
        rtn = rtn->nextBrother;
    }
    return rtn;
}

// pre-condicion: Existe el archivo "nombreArchivo" en la lista "archivos" en el directorio y "nombreArchivo" no es el primer nodo de la lista
// pos-condicion: Devuelve el nodo anterior al nodo de la lista que contiene el archivo "nombreArchivo"
LArchivos getPreviousFile(LArchivos archivos, Cadena nombreArchivo)
{
    LArchivos nodoFile = archivos;
    if (!(strcmp(getFileName(nodoFile->sig->file), nombreArchivo) == 0))
    {
        nodoFile = getPreviousFile(archivos->sig, nombreArchivo);
    }
    return nodoFile;
}

// pre-condicion: el directorio de nombre nombreDirectorio es hijo del directorio directorio
// pos-condicion: elimina el directorio de nombre nombreDirectorio que es hijo del directorio directorio
// eliminando toda su memoria
void removeChildrenDirectory(TDirectorio &directorio, Cadena nombreDirectorio)
{
    directorio->currentDirectory = moveChildrenDirectory(directorio, nombreDirectorio);
    if (!strcmp(directorio->firstSibling->name, nombreDirectorio) == 0)
    {
        TDirectorio anterior = getPreviousBrother(directorio, nombreDirectorio);
        anterior->nextBrother = directorio->currentDirectory->nextBrother;
        anterior = NULL;
    }
    else
    {
        directorio->firstSibling = directorio->currentDirectory->nextBrother;
    }
    destroyDirectory(directorio->currentDirectory);
    directorio->currentDirectory = directorio;
}

// pre-condicion: el directorio origen es sub-directorio del directorio "directorio"
// pos-condicion: mueve el directorio origen y todo su contenido al directorio destino
void moveSubDirectory(TDirectorio &directorio, TDirectorio origen, TDirectorio &destino)
{
    if (directorio->firstSibling != origen)
    {
        TDirectorio previous = getPreviousBrother(directorio, origen->name);
        previous->nextBrother = origen->nextBrother;
        previous = NULL;
    }
    else
    {
        directorio->firstSibling = origen->nextBrother;
    }

    TDirectorio firstSibling = destino->firstSibling;
    if (firstSibling == NULL || (strcmp(origen->name, firstSibling->name) < 0))
    {
        origen->nextBrother = firstSibling;
        destino->firstSibling = origen;
    }
    else if (strcmp(origen->name, firstSibling->name) == 0)
    {
        origen->nextBrother = firstSibling->nextBrother;
        destino->firstSibling = origen;
        destroyDirectory(firstSibling);
    }
    else
    {
        bool notInserted = true;
        while (firstSibling->nextBrother != NULL && notInserted)
        {
            if (strcmp(origen->name, firstSibling->nextBrother->name) < 0)
            {
                if (strcmp(origen->name, firstSibling->name) == 0)
                {
                    TDirectorio previous = getPreviousBrother(directorio, origen->name);
                    previous->nextBrother = origen;
                    origen->nextBrother = firstSibling->nextBrother;
                    destroyDirectory(firstSibling);
                    previous = NULL;
                }
                else
                {
                    origen->nextBrother = firstSibling->nextBrother;
                    firstSibling->nextBrother = origen;
                }

                notInserted = false;
            }
            firstSibling = firstSibling->nextBrother;
        }

        if (notInserted && firstSibling->nextBrother == NULL)
        {
            if (strcmp(origen->name, firstSibling->name) == 0)
            {
                TDirectorio previous = getPreviousBrother(directorio, origen->name);
                previous->nextBrother = origen;
                origen->nextBrother = firstSibling->nextBrother;
                destroyDirectory(firstSibling);
                previous = NULL;
            }
            else
            {
                origen->nextBrother = firstSibling->nextBrother;
                firstSibling->nextBrother = origen;
            }
        }
    }
    origen->father = destino;
    firstSibling = NULL;
}

// pre-condicion el archivo origen es sub archivo del directorio directorio
// pos-condicion se mueve el archivo TArchivo como hijo del directorio destino
void moveSubArchive(TDirectorio &directorio, TArchivo origen, TDirectorio destino)
{
    LArchivos nodeToMove = directorio->archivos;
    if (!strcmp(getFileName(nodeToMove->file), getFileName(origen)) == 0)
    {
        while (!strcmp(getFileName(nodeToMove->file), getFileName(origen)) == 0)
        {
            nodeToMove = nodeToMove->sig;
        }
        LArchivos previous = getPreviousFile(directorio->archivos, getFileName(origen));
        previous->sig = nodeToMove->sig;
        previous = NULL;
    }
    else
    {
        directorio->archivos = nodeToMove->sig;
    }

    LArchivos firstFile = destino->archivos;
    if (firstFile == NULL || (strcmp(getFileName(origen), getFileName(firstFile->file)) < 0))
    {
        nodeToMove->sig = destino->archivos;
        destino->archivos = nodeToMove;
    }
    else if (strcmp(getFileName(origen), getFileName(firstFile->file)) == 0)
    {
        nodeToMove->sig = firstFile->sig;
        destino->archivos = nodeToMove;
        destroyFile(firstFile->file);
        delete firstFile;
    }
    else
    {
        bool notInserted = true;
        while (firstFile->sig != NULL && notInserted)
        {
            if (strcmp(getFileName(origen), getFileName(firstFile->sig->file)) < 0)
            {
                if (strcmp(getFileName(origen), getFileName(firstFile->file)) == 0)
                {
                    LArchivos previous = getPreviousFile(destino->archivos, getFileName(firstFile->file));
                    previous->sig = nodeToMove;
                    nodeToMove->sig = firstFile->sig;
                    destroyFile(firstFile->file);
                    delete firstFile;
                    previous = NULL;
                }
                else
                {
                    nodeToMove->sig = firstFile->sig;
                    firstFile->sig = nodeToMove;
                }

                notInserted = false;
            }
            firstFile = firstFile->sig;
        }

        if (notInserted && firstFile->sig == NULL)
        {
            if (strcmp(getFileName(origen), getFileName(firstFile->file)) == 0)
            {
                LArchivos previous = getPreviousFile(destino->archivos, getFileName(firstFile->file));
                previous->sig = nodeToMove;
                nodeToMove->sig = firstFile->sig;
                destroyFile(firstFile->file);
                delete firstFile;
                previous = NULL;
            }
            else
            {
                nodeToMove->sig = firstFile->sig;
                firstFile->sig = nodeToMove;
            }
        }
    }

    nodeToMove = NULL;
    firstFile = NULL;
}

// pre-condicion: directorio no es el directorio ROOT
// pos-condicion: retorna un puntero al primer hermano del diretorio "directorio"
TDirectorio firstBrotherDirectory(TDirectorio directorio)
{
    return directorio->nextBrother;
}

// pre-condicion: Directorio != NULL
// pos-condicion: retorna un puntero al primer hijo del directorio "directorio"
TDirectorio firstChildrenDirectory(TDirectorio directorio)
{
    return directorio->firstSibling;
}

// Retorna true si el directorio subdir es sub-directorio del directorio "directorio" en cualquier nivel.
bool isSubDirectoryRoot(TDirectorio directorio, Cadena ruta)
{
    bool rtn = false;

    // Se crea copia de ruta
    Cadena destino = new char[strlen(ruta)];
    destino = strcpy(destino, ruta);

    // Lee primer directorio
    destino = strtok(destino, SLASH);

    // En caso de coincidir el primer directorio de la ruta con el actual, avanza al siguiente
    destino = strcmp(destino, directorio->name) == 0 ? strtok(NULL, SLASH) : destino;

    bool flag = true;
    while (destino != NULL && flag)
    {
        if (existChildrenDirectory(directorio, destino))
        {
            directorio = moveChildrenDirectory(directorio, destino);
            destino = strtok(NULL, SLASH);
            rtn = destino == NULL;
        }
        else
        {
            flag = false;
        }
    }

    delete[] destino;
    destino = NULL;
    return rtn;
}

// Auxiliar para imprimir permisos del archivo "archivo"
Cadena getTextOfPermission(TArchivo archivo)
{
    Cadena rtn = haveWritePermission(archivo) ? new char[strlen(WRITE_PERMISSION) + 1] : new char[strlen(READ_PERMISSION) + 1];
    rtn = haveWritePermission(archivo) ? strcpy(rtn, WRITE_PERMISSION) : strcpy(rtn, READ_PERMISSION);
    return rtn;
}

// pre-condicion: directorio != NULL
// pos-condicion: retorna una cadena conteniendo la ruta de "directorio"
Cadena getRootTrace(TDirectorio directorio)
{
    int length;
    Cadena trace;
    if (directorio->father != NULL)
    {
        Cadena rtn = getRootTrace(directorio->father);
        length = strlen(rtn) + strlen(directorio->name) + 1;
        trace = new char[length];
        trace = strcpy(trace, rtn);
        trace = strcat(trace, SLASH);
        trace = strcat(trace, directorio->name);
    }
    else
    {
        length = strlen(directorio->name) + 1;
        trace = new char[length];
        trace = strcpy(trace, directorio->name);
    }

    return trace;
}

// Auxiliar para imprimir archivos del directorio "directorio" con el formato directorio->name/archivo->name
void printFilesNameInCurrentDirectoryDir(TDirectorio directorio)
{
    if (!isEmptyDirectory(directorio))
    {
        LArchivos files = directorio->archivos;
        while (files != NULL)
        {
            printf("%-30s%s\n", getFileName(files->file), getTextOfPermission(files->file));
            files = files->sig;
        }
        files = NULL;
    }
}

// pre-condicion: directorio != NULL
// Auxiliar para imprimir archivos del directorio "directorio" con el formato directorio->name/archivo->name
void printFilesNameInCurrentDirectoryDirS(TDirectorio directorio)
{
    if (!isEmptyDirectory(directorio))
    {
        Cadena trace = NULL;
        if (!isRootDirectory(directorio))
        {
            trace = getRootTrace(directorio);
        }

        LArchivos files = directorio->archivos;
        while (files != NULL)
        {
            if (!isRootDirectory(directorio))
            {
                printf("%s/%s\n", trace, getFileName(files->file));
            }
            else
            {
                printf("%s/%s\n", directorio->name, getFileName(files->file));
            }
            files = files->sig;
        }
        if (trace != NULL)
        {
            delete[] trace;
            trace = NULL;
        }
        files = NULL;
    }
}

// pos-condicion: imprime el directorio ejecuando DIR
void printDirectoryDir(TDirectorio directorio)
{
    if (!isRootDirectory(directorio))
    {
        Cadena trace = getRootTrace(directorio);
        printf("%s\n", trace);
        delete[] trace;
    }
    else
    {
        printf("%s\n", directorio->name);
    }

    if (!isEmptyDirectory(directorio) || directorio->firstSibling != NULL)
    {
        printFilesNameInCurrentDirectoryDir(directorio);
        TDirectorio iter = directorio->firstSibling;
        while (iter != NULL)
        {
            printf("%s\n", iter->name);
            iter = iter->nextBrother;
        }
    }
    else
    {
        printf("No existen archivos ni directorios.\n");
    }
}

// pos-condicion: imprime los hijos y hermanos de directorio
void printSubDirectoriesDirS(TDirectorio directorio)
{
    if (directorio != NULL)
    {
        Cadena trace = getRootTrace(directorio);
        printf("%s\n", trace);
        printFilesNameInCurrentDirectoryDirS(directorio);
        printSubDirectoriesDirS(directorio->firstSibling);
        printSubDirectoriesDirS(directorio->nextBrother);
        if (trace != NULL)
        {
            delete[] trace;
        }
    }
}

// pos-condicion: imprime el directorio ejecutando DIR /S
void printDirectoryDirS(TDirectorio directorio)
{
    if (isRootDirectory(directorio))
    {
        printf("%s\n", directorio->name); // NOMBRE
        if (!isEmptyDirectory(directorio) || directorio->firstSibling != NULL)
        {
            printFilesNameInCurrentDirectoryDirS(directorio); // ARCHIVOS
            if (directorio->firstSibling != NULL)             // HIJOS
            {
                printSubDirectoriesDirS(directorio->firstSibling);
            }
        }
        else
        {
            printf("No existen archivos ni directorios.\n");
        }
    }
    else
    {
        Cadena trace = getRootTrace(directorio);
        printf("%s\n", trace);
        if (!isEmptyDirectory(directorio) || directorio->firstSibling != NULL)
        {
            printFilesNameInCurrentDirectoryDirS(directorio); // Archivos
            printSubDirectoriesDirS(directorio->firstSibling);
        }
        if (trace != NULL)
        {
            delete[] trace;
        }
    }
}