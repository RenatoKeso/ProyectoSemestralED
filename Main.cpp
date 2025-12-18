#include <iostream>
#include <vector>
#include <algorithm>   

#include "Lector_CSV.h"
#include "AirRecord.h"
#include "BT.h"
#include "BST.h"
#include "AVL.h"

using namespace std;

int main() {
    vector<AirRecord> records;

    if (!loadAirQualityCSV("Air_Quality.csv", records)) {
        cerr << "No se pudo leer el CSV\n";
        return 1;
    }

    cout << "Registros cargados: " << records.size() << "\n\n";

    if (records.empty()) {
        cerr << "No hay registros para procesar.\n";
        return 1;
    }

    int n = (int)records.size();
    int idExistente = records[0].uniqueID;
    int idNoExistente = 999999999;

    // 2) AVL
    AVL arbolAVL;

    cout << "Insertando " << n << " registros en el AVL...\n";
    for (int i = 0; i < n; i++){
        arbolAVL.insertar(records[i]);
    }
    cout << "Insercion completada (AVL).\n";

    cout << "Buscando ID existente (AVL): " << idExistente << " -> "
        << (arbolAVL.buscar(idExistente) ? "Encontrado" : "No encontrado") << "\n";

    cout << "Buscando ID no existente (AVL): " << idNoExistente << " -> "
        << (arbolAVL.buscar(idNoExistente) ? "Encontrado" : "No encontrado") << "\n";

    cout << "Eliminando ID existente (AVL): " << idExistente << "\n";
    arbolAVL.remove(idExistente);

    cout << "Buscar ID despues de eliminar (AVL): "
        << (arbolAVL.buscar(idExistente) ? "Encontrado" : "No encontrado") << "\n\n";

    // 3) BST
    BST arbolBST;

    cout << "Insertando " << n << " registros en el BST...\n";
    for (int i = 0; i < n; i++) {
        arbolBST.insertar(records[i]);
    }
    cout << "Insercion completada (BST).\n";

    cout << "Buscando ID existente (BST): " << idExistente << " -> "
        << (arbolBST.buscar(idExistente) ? "Encontrado" : "No encontrado") << "\n";

    cout << "Buscando ID no existente (BST): " << idNoExistente << " -> "
        << (arbolBST.buscar(idNoExistente) ? "Encontrado" : "No encontrado") << "\n";

    cout << "Eliminando ID existente (BST): " << idExistente << "\n";
    arbolBST.eliminar(idExistente);

    cout << "Buscando ID eliminado (BST): " << idExistente << " -> "
        << (arbolBST.buscar(idExistente) ? "Encontrado" : "No encontrado") << "\n\n";

    // 4) BT
    BT arbolBT;
    cout << "Insertando 10 registros en el BT...\n";
    for (int i = 0; i < 10 && i < n; i++){
        arbolBT.insertar(records[i]);
    }

    cout << "Recorrido InOrden del BT:\n";
    arbolBT.inOrden();
    cout << "\n";

    cout << "Algunos registros cargados:\n";
    for (int i = 0; i < min(5, n); i++) {
        cout << "ID Unico: " << records[i].uniqueID << "\n";
        cout << "Geo Place Name: " << records[i].geoPlaceName << "\n";
        cout << "Data Value: " << records[i].dataValue << "\n";
        cout << "---------------------\n";
    }

    return 0;
}
