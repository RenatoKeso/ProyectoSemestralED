#ifndef AVL_H
#define AVL_H

#include "AirRecord.h"
#include <iostream>
#include <algorithm>
using namespace std;

struct AVLNodo{
    AirRecord dato;
    AVLNodo* izq;
    AVLNodo* der;
    int altura;

    AVLNodo(const AirRecord& r): dato(r), izq(nullptr), der(nullptr), altura(1) {}
};

class AVL{
private:
    AVLNodo* raiz;

    int h(AVLNodo* n) const { return n ? n->altura : 0; }

    int factorBalance(AVLNodo* n) const {
        return n ? (h(n->izq) - h(n->der)) : 0;
    }

    void actualizarAltura(AVLNodo* n){
        n->altura = 1 + max(h(n->izq), h(n->der));
    }

    AVLNodo* rotacionDerecha(AVLNodo* y){
        AVLNodo* x = y->izq;
        AVLNodo* T2 = x->der;

        x->der = y;
        y->izq = T2;

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    AVLNodo* rotacionIzquierda(AVLNodo* x){
        AVLNodo* y = x->der;
        AVLNodo* T2 = y->izq;

        y->izq = x;
        x->der = T2;

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }

    AVLNodo* rebalancear(AVLNodo* nodo){
        actualizarAltura(nodo);
        int balance = factorBalance(nodo);

        // Left heavy
        if (balance > 1){
            // Left-Right
            if (factorBalance(nodo->izq) < 0)
                nodo->izq = rotacionIzquierda(nodo->izq);
            // Left-Left
            return rotacionDerecha(nodo);
        }

        // Right heavy
        if (balance < -1){
            // Right-Left
            if (factorBalance(nodo->der) > 0)
                nodo->der = rotacionDerecha(nodo->der);
            // Right-Right
            return rotacionIzquierda(nodo);
        }

        return nodo;
    }

    AVLNodo* insertarRec(AVLNodo* nodo, const AirRecord& r){
        if (!nodo) return new AVLNodo(r);

        if (r.uniqueID < nodo->dato.uniqueID)
            nodo->izq = insertarRec(nodo->izq, r);
        else if (r.uniqueID > nodo->dato.uniqueID)
            nodo->der = insertarRec(nodo->der, r);
        else
            return nodo; // no duplicados

        return rebalancear(nodo);
    }

    AVLNodo* minimoNodo(AVLNodo* nodo){
        while (nodo && nodo->izq) nodo = nodo->izq;
        return nodo;
    }

    AVLNodo* eliminarRec(AVLNodo* nodo, int key){
        if (!nodo) return nullptr;

        if (key < nodo->dato.uniqueID){
            nodo->izq = eliminarRec(nodo->izq, key);
        } else if (key > nodo->dato.uniqueID){
            nodo->der = eliminarRec(nodo->der, key);
        } else {
            // encontrado
            if (!nodo->izq && !nodo->der){
                delete nodo;
                return nullptr;
            }
            if (!nodo->izq){
                AVLNodo* temp = nodo->der;
                delete nodo;
                return temp;
            }
            if (!nodo->der){
                AVLNodo* temp = nodo->izq;
                delete nodo;
                return temp;
            }

            // 2 hijos: sucesor
            AVLNodo* succ = minimoNodo(nodo->der);
            nodo->dato = succ->dato;
            nodo->der = eliminarRec(nodo->der, succ->dato.uniqueID);
        }

        // rebalanceo al volver
        return rebalancear(nodo);
    }

    AVLNodo* buscarRec(AVLNodo* nodo, int key) const {
        if (!nodo) return nullptr;
        if (key == nodo->dato.uniqueID) return nodo;
        if (key < nodo->dato.uniqueID) return buscarRec(nodo->izq, key);
        return buscarRec(nodo->der, key);
    }

    void eliminarArbol(AVLNodo* nodo){
        if (!nodo) return;
        eliminarArbol(nodo->izq);
        eliminarArbol(nodo->der);
        delete nodo;
    }

    void inOrdenRec(AVLNodo* nodo) const {
        if (!nodo) return;
        inOrdenRec(nodo->izq);
        cout << nodo->dato.uniqueID << " ";
        inOrdenRec(nodo->der);
    }

public:
    AVL(): raiz(nullptr) {}
    ~AVL(){ eliminarArbol(raiz); }

    void insertar(const AirRecord& r){ raiz = insertarRec(raiz, r); }

    bool buscar(int key) const { return buscarRec(raiz, key) != nullptr; }

    void remove(int key){ raiz = eliminarRec(raiz, key); }

    void printInOrden() const {
        inOrdenRec(raiz);
        cout << "\n";
    }
};

#endif
