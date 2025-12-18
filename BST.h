#ifndef BST_H
#define BST_H

#include "AirRecord.h"
#include <iostream>
using namespace std;

struct BSTNodo{
    AirRecord dato;
    BSTNodo* izq;
    BSTNodo* der;
    BSTNodo(const AirRecord& r): dato(r), izq(nullptr), der(nullptr) {}
};

class BST{
    private: 
    BSTNodo* raiz;

    void eliminarRec(BSTNodo* nodo){
        if (!nodo) return;
        eliminarRec(nodo->izq);
        eliminarRec(nodo->der);
        delete nodo;
    }

    void insertarRec(BSTNodo*& nodo, const AirRecord& r){
        if (nodo == nullptr){
            nodo = new BSTNodo(r);
            return;
        }
        if (r.uniqueID < nodo->dato.uniqueID) insertarRec(nodo->izq, r);
        else if (r.uniqueID > nodo->dato.uniqueID) insertarRec(nodo->der, r);
        else {
            // IDs iguales, no insertamos duplicados
        }
    }

    BSTNodo* buscarRec(BSTNodo* nodo, int key)const {
        if (!nodo) return nullptr;
        if (key == nodo->dato.uniqueID) return nodo;
        if (key < nodo->dato.uniqueID) return buscarRec(nodo->izq, key);
        return buscarRec(nodo->der, key);
    }

    BSTNodo* minimoNodo(BSTNodo* nodo){
        while (nodo && nodo->izq) nodo = nodo->izq;
        return nodo;
    }

    // Eliminamos un solo nodo (para mantener los subarboles con sucesor in-orden)
    BSTNodo* eliminarRec(BSTNodo* nodo, int key){
        if (!nodo) return nullptr;
        if (key < nodo->dato.uniqueID){
            nodo->izq = eliminarRec(nodo->izq, key);
        } else if (key > nodo->dato.uniqueID){
            nodo->der = eliminarRec(nodo->der, key);
        } else {
            //Si encontramos el nodo
            if (!nodo->izq && !nodo->der){
                delete nodo;
                return nullptr;                
            }
            if (!nodo->izq){
                BSTNodo* temp = nodo->der;
                delete nodo;
                return temp;
            }
            if (!nodo->der){
                BSTNodo* temp = nodo->izq;
                delete nodo;
                return temp;
            }

            // 2 hijos: buscamos sucesor in-orden
            BSTNodo* sucesor = minimoNodo(nodo->der);
            nodo->dato = sucesor->dato;
            nodo->der = eliminarRec(nodo->der, sucesor->dato.uniqueID);
        }
        return nodo;
    }
    
    void inOrdenRec(BSTNodo* nodo)const {
        if (!nodo) return;
        inOrdenRec(nodo->izq);
        cout << nodo->dato.uniqueID << " ";
        inOrdenRec(nodo->der);
    }
    
    public: 
    BST(): raiz(nullptr) {}
    ~BST(){eliminarRec(raiz);}

    void insertar(const AirRecord& r) {insertarRec(raiz, r);}
    
    bool buscar(int key)const { return buscarRec(raiz, key) != nullptr; }

    // Si queremos recuperar todo el registro por su key
    const AirRecord* buscarRegistro(int key)const {
        BSTNodo* n = buscarRec(raiz, key);
        return n ? &n->dato : nullptr;
    }

    void eliminar(int key) {raiz = eliminarRec(raiz, key);}

    void inOrden() const{
        inOrdenRec(raiz);
        cout << "\n";
    }
};
#endif