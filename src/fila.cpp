/* 1111111 */ // sustituiir con los 7 dígitos de la cédula
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../include/fila.h"
#include "../include/linea.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef char *Cadena;

struct _rep_fila
{
    TLinea elemento;
    TFila sig;
};

// Pos-condición crea una fila vacía.
TFila createRow()
{
    return nullptr;
}

// Pos-Condición retorna true si la fila es vacía.
bool isEmptyRow(TFila fila)
{
    return fila == nullptr;
}

TLinea copyLine(TLinea linea)
{
    TLinea newLine = createLine();
    while (!isEmptyLine(linea))
    {
        insertCharLine(firstCharLine(linea), newLine);
        linea = nextLine(linea);
    }
    return newLine;
}

// Pos-Condición Inserta una nueva fila al principio de "fila"
void insertRow(TFila &fila, TLinea linea)
{
    TFila newFila = new _rep_fila;
    newFila->elemento = copyLine(linea); // Para testear por uso compartido de memoria
    newFila->sig = fila;
    fila = newFila;
    newFila = nullptr;
}

// pre-condicion: fila !=NULL
// pos-condicion: retorna un puntero al primer nodo de la fila "fila"
TLinea headRow(TFila fila)
{
    return fila->elemento;
}

// pre-condicion: fila !=NULL
// pos-condicion: Retorna un puntero al siguente elemento de la fila "fila"
TFila nextRow(TFila fila)
{
    return fila->sig;
}

// pre-condicion: fila != NULL
// elimina el ultimo nodo de la fila "fila"
void deleteLastRow(TFila &fila)
{
    if (fila->sig != nullptr) // Tiene mas de un nodo
    {
        TFila auxMenosUno = fila;
        while (auxMenosUno->sig->sig != nullptr)
        {
            auxMenosUno = auxMenosUno->sig;
        }
        TFila auxDelete = auxMenosUno->sig; // Avanzo a ultimo
        if (!isEmptyLine(headRow(auxDelete)))
        {
            TLinea aux = headRow(auxDelete);
            destroyLine(aux);
            aux = NULL;
        }
        auxMenosUno->sig = NULL;
        delete auxDelete;
        auxDelete = NULL;
    }
    else // Solo un nodo
    {
        TFila auxDelete = fila;
        fila = NULL;
        destroyLine(auxDelete->elemento);
        delete auxDelete;
        auxDelete = NULL;
    }
}

// Elimina toda la memoria de la fila "fila"
void deleteRows(TFila &fila)
{
    if (fila != nullptr)
    {

        while (fila->sig != nullptr)
        {
            TFila auxDelete = fila;
            fila = auxDelete->sig;
            destroyLine(auxDelete->elemento);
            delete auxDelete;
            auxDelete = NULL;
        }

        TFila aux = fila;
        fila = NULL;
        destroyLine(aux->elemento);
        delete aux;
        aux = NULL;
    }
}

// Pos-Condición modifica el primer elemento de la fila "fila" agregando los caracteres al inicio del primer elemento "linea" de la fila.
void modifyRow(TFila &fila, Cadena caracteres)
{

    if (fila->elemento != NULL)
    {
        Cadena aux = new char[strlen(caracteres)];
        aux = strcpy(aux, caracteres);

        TLinea auxL = headRow(fila);

        int largo = strlen(aux) - 1;
        while (largo > 0)
        {
            insertCharLine(aux[largo], auxL);
            largo--;
        }

        aux = nullptr;
        auxL = nullptr;
    }
}