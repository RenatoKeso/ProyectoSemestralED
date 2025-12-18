#ifndef BT_H
#define BT_H

#include "AirRecord.h"
#include <iostream>
#include <queue>

using namespace std;

struct BTNodo {
    AirRecord dato;
    BTNodo* izq;
    BTNodo* der;

    BTNodo(const AirRecord& r) : dato(r), izq(nullptr), der(nullptr) {}
};

class BT {
private:
    BTNodo* raiz;

    // Inserción NO ordenada (simple)
    void insertarRec(BTNodo*& nodo, const AirRecord& r) {
        if (nodo == nullptr) {
            nodo = new BTNodo(r);
            return;
        }

        if (nodo->izq == nullptr) insertarRec(nodo->izq, r);
        else insertarRec(nodo->der, r);
    }

    // Recorrido in-order (prueba)
    void inOrdenRec(BTNodo* nodo) const {
        if (!nodo) return;
        inOrdenRec(nodo->izq);
        cout << nodo->dato.uniqueID << " ";
        inOrdenRec(nodo->der);
    }

    // Liberar memoria
    void eliminarRec(BTNodo* nodo) {
        if (!nodo) return;
        eliminarRec(nodo->izq);
        eliminarRec(nodo->der);
        delete nodo;
    }

public:
    BT() : raiz(nullptr) {}

    ~BT() { eliminarRec(raiz); }

    void insertar(const AirRecord& r) { insertarRec(raiz, r); }

    void inOrden() const {
        inOrdenRec(raiz);
        cout << "\n";
    }

    
    // buscar y eliminar (para benchmark)
    

    // Buscar O(n): recorrido BFS con cola
    bool buscar(int key) const {
        if (!raiz) return false;

        queue<BTNodo*> q;
        q.push(raiz);

        while (!q.empty()) {
            BTNodo* cur = q.front();
            q.pop();

            if (cur->dato.uniqueID == key) return true;

            if (cur->izq) q.push(cur->izq);
            if (cur->der) q.push(cur->der);
        }
        return false;
    }

    // Eliminar un nodo SIN eliminar subárboles:
    // Técnica estándar en BT: reemplazar el nodo objetivo por el "deepest-rightmost"
    // y luego borrar ese último nodo.
    void eliminar(int key) {
        if (!raiz) return;

        // Caso especial: si solo hay raíz y coincide
        if (!raiz->izq && !raiz->der) {
            if (raiz->dato.uniqueID == key) {
                delete raiz;
                raiz = nullptr;
            }
            return;
        }

        queue<BTNodo*> q;
        q.push(raiz);

        BTNodo* objetivo = nullptr;      // nodo que queremos eliminar
        BTNodo* ultimo = nullptr;        // último nodo en BFS (deepest-rightmost)
        BTNodo* padreUltimo = nullptr;   // padre del último

        while (!q.empty()) {
            BTNodo* cur = q.front();
            q.pop();

            if (cur->dato.uniqueID == key) objetivo = cur;

            if (cur->izq) {
                padreUltimo = cur;
                ultimo = cur->izq;
                q.push(cur->izq);
            }
            if (cur->der) {
                padreUltimo = cur;
                ultimo = cur->der;
                q.push(cur->der);
            }
        }

        if (!objetivo || !ultimo) return;

        // Copiamos el dato del último al objetivo
        objetivo->dato = ultimo->dato;

        // Desconectamos y borramos el último nodo
        if (padreUltimo) {
            if (padreUltimo->izq == ultimo) padreUltimo->izq = nullptr;
            else if (padreUltimo->der == ultimo) padreUltimo->der = nullptr;
        }
        delete ultimo;
    }
};

#endif
