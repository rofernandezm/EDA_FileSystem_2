/*
Módulo principal de la tarea 1.FILE SYSTEM

Intérprete de comandos para probar los módulos.

  Laboratorio de EDA 2023
  UTU - LATU - Turno Nocturno
  Profesor Ferando Arrieta
 */

#include "include/archivo.h"
#include "include/linea.h"
#include "include/fila.h"
#include "include/directorio.h"
#include "include/utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstddef>
#define MAX_PALABRA 64
#define MAX_LINEA 256
#define MAX_RUTA 256
#define MAX_PARAMETRO 10
#define MAX_PARAMETRODIR 1
#define MAX_COMANDO 10
#define MAX_NOMBRE 15
#define MAX_EXTENSION 3
#define TEXTO_MAX 50

enum _retorno
{
    OK,
    ERROR,
    NO_IMPLEMENTADA
};
typedef enum _retorno TipoRet;
typedef char *Cadena;

// Testtt
TipoRet CREARSISTEMA(TDirectorio &s);
TipoRet CREATE(TDirectorio &sistema, char *nombreArchivo);
TipoRet DELETE(TDirectorio &sistema, char *nombreArchivo);
TipoRet ATTRIB(TDirectorio &sistema, char *nombreArchivo, char *parametro);
TipoRet IF(TDirectorio &sistema, char *nombreArchivo, char *texto);
TipoRet IN(TDirectorio &sistema, char *nombreArchivo, char *texto);
TipoRet DF(TDirectorio &sistema, char *nombreArchivo, char *cantidad);
TipoRet TYPE(TDirectorio &sistema, char *nombreArchivo);
TipoRet DESTRUIRSISTEMA(TDirectorio &sistema);
TipoRet CD(TDirectorio &sistema, Cadena nombreDirectorio);
TipoRet MKDIR(TDirectorio &sistema, Cadena nombreDirectorio);
TipoRet RMDIR(TDirectorio &sistema, Cadena nombreDirectorio);
TipoRet MOVE(TDirectorio &sistema, Cadena nombre, Cadena directorioDestino);
TipoRet DIR(TDirectorio &sistema, Cadena parametroDir);
// programa principal

int main()
{
    char restoLinea[MAX_LINEA + 1];
    char comando[MAX_PALABRA];
    char parametro[MAX_PALABRA];
    char parametroDir[MAX_PARAMETRODIR];
    char texto[MAX_PALABRA];
    char nombrearchivo[MAX_NOMBRE];
    char nombreDirectorio[MAX_NOMBRE];
    char cantidad[MAX_PALABRA];
    // char parametro;
    TDirectorio sistema;
    int sistemaInicializado = false;
    bool salir = false;
    while (!salir)
    {
        printf("#");
        // leer el comando
        leerChars(comando);
        if (!strcmp(comando, "ATTRIB"))
        {
            leerChars(nombrearchivo);
            leerChars(parametro);
        }
        else if (!strcmp(comando, "CREATEFILE"))
            leerChars(nombrearchivo);
        else if (!strcmp(comando, "DELETE"))
            leerChars(nombrearchivo);
        else if (!strcmp(comando, "IF"))
        {
            leerChars(nombrearchivo);
            leerRestoLinea(texto);
        }
        else if (!strcmp(comando, "IN"))
        {
            leerChars(nombrearchivo);
            leerRestoLinea(texto);
        }
        else if (!strcmp(comando, "DF"))
        {
            leerChars(nombrearchivo);
            leerChars(cantidad);
        }
        else if (!strcmp(comando, "TYPE"))
        {
            leerChars(nombrearchivo);
        }
        else if (!strcmp(comando, "CD"))
        {
            leerChars(nombreDirectorio);
        }
        else if (!strcmp(comando, "MKDIR"))
        {
            leerChars(nombreDirectorio);
        }
        else if (!strcmp(comando, "RMDIR"))
        {
            leerChars(nombreDirectorio);
        }
        else if (!strcmp(comando, "MOVE"))
        {
            leerChars(nombrearchivo);
            leerChars(nombreDirectorio);
        }
        else if (!strcmp(comando, "DIR"))
        {
            leerRestoLinea(parametroDir);
        }
        // *********************** Procesamiento de comandos ***********************************

        if (0 == strcmp(comando, "CREARSISTEMA"))
        {
            if (!sistemaInicializado)
            {
                TipoRet salida = CREARSISTEMA(sistema);
                if (salida == OK)
                {
                    sistemaInicializado = true;
                    printf("*** SYSTEM INICIALIZED ***\n");
                    printf("OK\n\n");
                }
            }
            else
                printf("EL SISTEMA YA FUE INICIALIZADO\n\n");
        }
        else if (0 == strcmp(comando, "CREATEFILE"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = CREATE(sistema, nombrearchivo);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                {
                    printf("NO_IMPLEMENTADA\n\n");
                }
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "DELETE"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = DELETE(sistema, nombrearchivo);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "ATTRIB"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = ATTRIB(sistema, nombrearchivo, parametro);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "IF"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = IF(sistema, nombrearchivo, texto);
                if (salida == OK)
                    printf("\nOK\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "IN"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = IN(sistema, nombrearchivo, texto);
                if (salida == OK)
                    printf("\nOK\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "DF"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = DF(sistema, nombrearchivo, cantidad);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "TYPE"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = TYPE(sistema, nombrearchivo);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "CD"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = CD(sistema, nombreDirectorio);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "MKDIR"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = MKDIR(sistema, nombreDirectorio);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "RMDIR"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = RMDIR(sistema, nombreDirectorio);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "MOVE"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = MOVE(sistema, nombrearchivo, nombreDirectorio);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "DIR"))
        {
            if (sistemaInicializado)
            {
                TipoRet salida = DIR(sistema, parametroDir);
                if (salida == OK)
                    printf("\nOK\n\n");
                else if (salida == NO_IMPLEMENTADA)
                    printf("NO_IMPLEMENTADA\n\n");
            }
            else
            {
                printf("\nEL SISTEMA NO FUE INICIALIZADO\n\n");
            }
        }
        else if (0 == strcmp(comando, "DESTRUIRSISTEMA"))
        {
            TipoRet salida;
            if (sistemaInicializado)
            {
                salida = DESTRUIRSISTEMA(sistema);
            }
            else
            {
                salida = OK;
            }
            if (salida == OK)
            {
                sistemaInicializado = false;
                printf("*** SYSTEM DESTROYED ***\n\n");
                printf("OK\n");
            }
            else if (salida == NO_IMPLEMENTADA)
                printf("NO_IMPLEMENTADA\n\n");
        }
        else
            printf("Comando no reconocido.\n\n");

        fgets(restoLinea, MAX_LINEA + 1, stdin);
    } // while
    return 0;
} // main

//****************************** Funciones a implementar ************************************

TipoRet CREARSISTEMA(TDirectorio &sistema)
{
    sistema = createRootDirectory();
    return OK;
}

TipoRet CREATE(TDirectorio &sistema, Cadena nombreArchivo)
{
    TipoRet salida = OK;
    if (!existFileDirectory(sistema, nombreArchivo))
    {
        createFileInDirectory(sistema, nombreArchivo);
        salida = OK;
    }
    else
    {
        printf("*** ERROR - Ya existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet DELETE(TDirectorio &sistema, Cadena nombreArchivo)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        if (haveWritePermission(getFileDirectory(sistema, nombreArchivo)))
        {
            deleteFileDirectory(sistema, nombreArchivo);
            salida = OK;
        }
        else
        {
            printf("*** ERROR PERMISO DENEGADO - El archivo con el nombre: \"%s\" unicamente posee permisos del tipo \"Lectura\".\n\n", nombreArchivo);
            salida = ERROR;
        }
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet ATTRIB(TDirectorio &sistema, Cadena nombreArchivo, Cadena parametro)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        bool permission = strcmp(parametro, "+W") == 0 ? true : false;
        setFilePermission(sistema, nombreArchivo, permission);
        salida = OK;
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet IF(TDirectorio &sistema, Cadena nombreArchivo, Cadena texto)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        if (haveWritePermission(getFileDirectory(sistema, nombreArchivo)))
        {
            // LLamado a funciones
            insertCharsFileFirstLine(sistema, nombreArchivo, texto);
            TArchivo file = getFileDirectory(sistema, nombreArchivo);
            int chars = getCountChars(file);
            int diff = chars - TEXTO_MAX;
            if (diff > 0)
            {
                deleteCharsFile(sistema, nombreArchivo, diff);
            }
            file = NULL;
            salida = OK;
        }
        else
        {
            printf("*** ERROR PERMISO DENEGADO - El archivo con el nombre: \"%s\" unicamente posee permisos del tipo \"Lectura\".\n\n", nombreArchivo);
            salida = ERROR;
        }
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet IN(TDirectorio &sistema, Cadena nombreArchivo, Cadena texto)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        if (haveWritePermission(getFileDirectory(sistema, nombreArchivo)))
        {
            // LLamado a funciones
            insertTextFile(sistema, nombreArchivo, texto);
            TArchivo file = getFileDirectory(sistema, nombreArchivo);
            int chars = getCountChars(file);
            int diff = chars - TEXTO_MAX;
            if (diff > 0)
            {
                deleteCharsFile(sistema, nombreArchivo, diff);
            }
            file = NULL;
            salida = OK;
        }
        else
        {
            printf("*** ERROR PERMISO DENEGADO - El archivo con el nombre: \"%s\" unicamente posee permisos del tipo \"Lectura\".\n\n", nombreArchivo);
            salida = ERROR;
        }
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet DF(TDirectorio &sistema, Cadena nombreArchivo, Cadena cantidad)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        if (haveWritePermission(getFileDirectory(sistema, nombreArchivo)))
        {
            // LLamado a funciones
            int cant = atoi(cantidad);
            deleteCharsFile(sistema, nombreArchivo, cant);
            salida = OK;
        }
        else
        {
            printf("*** ERROR PERMISO DENEGADO - El archivo con el nombre: \"%s\" unicamente posee permisos del tipo \"Lectura\".\n\n", nombreArchivo);
            salida = ERROR;
        }
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet TYPE(TDirectorio &sistema, Cadena nombreArchivo)
{
    TipoRet salida;
    if (existFileDirectory(sistema, nombreArchivo))
    {
        if (!isEmptyFile(getFileDirectory(sistema, nombreArchivo)))
        {
            printFile(sistema, nombreArchivo);
            salida = OK;
        }
        else
        {
            printf("*** El archivo parámetro no posee contenido.\n\n");
            salida = ERROR;
        }
    }
    else
    {
        printf("*** ERROR - No existe un archivo con el nombre: \"%s\" en el directorio actual.\n\n", nombreArchivo);
        salida = ERROR;
    }
    return salida;
}

TipoRet DESTRUIRSISTEMA(TDirectorio &sistema)
{
    destroyDirectory(sistema);
    return OK;
}

TipoRet CD(TDirectorio &sistema, Cadena nombreDirectorio)
{
    return NO_IMPLEMENTADA;
}

TipoRet MKDIR(TDirectorio &sistema, Cadena nombreDirectorio)
{

    TipoRet salida;
    if (!strcmp(nombreDirectorio, "RAIZ") == 0)

    {
        if (!existChildrenDirectory(sistema, nombreDirectorio))
        {
            createChildrenDirectory(sistema, nombreDirectorio);
            salida = OK;
        }
        else
        {
            // IMPRIMIR DETALLE ERROR, YA EXISTE
            salida = ERROR;
        }
    }
    else
    {
        // IMPRIMIR DETALLE ERROR, NOMBREDIR = RAIZ

        salida = ERROR;
    }
    return salida;
}

TipoRet RMDIR(TDirectorio &sistema, Cadena nombreDirectorio)
{
    return NO_IMPLEMENTADA;
}

TipoRet MOVE(TDirectorio &sistema, Cadena nombre, Cadena directorioDestino)
{
    return NO_IMPLEMENTADA;
}

TipoRet DIR(TDirectorio &sistema, Cadena parametroDir)
{

    if (strlen(parametroDir) > 0)
    {
        for (int ind = 0; parametroDir[ind] != '\0'; ++ind)
        {
            parametroDir[ind] = parametroDir[ind + 1];
        }
    }

    if (strcmp(parametroDir, "/S") == 0)
    {
        printDirectoryDirS(sistema);
        // printf("DIR CON PARAMETRO\n");
        //  printf("PARAMETRO: \"%s\"\n", parametroDir);
    }
    else
    {
        printDirectoryDir(sistema);
    }

    return NO_IMPLEMENTADA;
}