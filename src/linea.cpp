/* 1111111 */ // sustituiir con los 7 dígitos de la cédula
/*
 Modulo que implemente el archivo linea.h

 */

#include "../include/linea.h"
#include <string.h>
#include <stdio.h>

struct _rep_linea
{
    char elemento;
    TLinea sig;
};

// Pos-condicion: Crea una linea vacía
TLinea createLine() // CELDA DUMMY
{
    TLinea newLinea = new _rep_linea;
    newLinea->sig = nullptr;
    return newLinea; // changed
}

// pos-condicion: Retorna true si la linea "linea" es vacia
bool isEmptyLine(TLinea linea)
{
    bool rtn = false;
    if (linea == nullptr || (linea->elemento == '\0' && linea->sig == nullptr))
    {
        rtn = true;
    }
    return rtn;
}

// pre-condicion linea !=NULL
// Retorna un puntero a al siguiente nodo de "linea"
TLinea nextLine(TLinea linea)
{
    return linea->sig;
}

// pre-condicion: linea != NULL
// Inserta el caracter letra al inicio de la linea.
void insertCharLine(char letra, TLinea &linea)
{

    TLinea newLinea = new _rep_linea;
    newLinea->elemento = letra;
    newLinea->sig = linea->sig;
    linea->sig = newLinea;
    newLinea = nullptr;
}

// pre-condición linea != NULL
// Retorna el primer caracter de la linea "linea"
char firstCharLine(TLinea linea) // CONTROLAR ANTES EL AVANCE DE CELDA DUMMY
{
    return linea->elemento;
}

// Retorna la cantidad de elementos que tiene la linea "linea"
int countNodesLine(TLinea linea)
{ // Verifica desde primer nodo (dummy)
    int rtn;
    if (linea == nullptr)
    {
        rtn = 0;
    }
    else if (linea->elemento == '\0')
    {
        rtn = countNodesLine(linea->sig);
    }
    else
    {
        rtn = 1 + countNodesLine(linea->sig);
    }
    return rtn;
}

// pre-condicion: linea != NULL
// pos-condicion: Elimina el primer nodo de la linea "linea"
void deleteFirstChar(TLinea &linea) // Asumimos linea completa (empieza por dummy)
{
    TLinea auxDelete = linea->sig;
    linea->sig = auxDelete->sig;
    delete auxDelete;
}

// pre-condicion: linea != NULL
// pos-condicion: Elimina el ultimo nodo de la linea "linea"
void deleteLastChar(TLinea &linea)
{
    TLinea auxMenosUno = linea;
    while (!isEmptyLine(linea->sig->sig))
    {
        auxMenosUno = auxMenosUno->sig;
    }
    TLinea auxDelete = auxMenosUno->sig;
    auxMenosUno->sig = NULL;
    delete auxDelete;
}

// Pos-condicion: Destruye toda la memoria utilizada por linea
void destroyLine(TLinea &linea)
{
    if (!isEmptyLine(linea))
    {
        destroyLine(linea->sig);
        delete linea;
    }
    linea = nullptr;
}
