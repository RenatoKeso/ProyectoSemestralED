#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <numeric>

#include "Lector_CSV.h"
#include "AirRecord.h"
#include "BT.h"
#include "BST.h"
#include "AVL.h"

using namespace std;
using namespace chrono;

static double promedio(const vector<double>& v) {
    if (v.empty()) return 0.0;
    return accumulate(v.begin(), v.end(), 0.0) / (double)v.size();
}

struct TiemposIter {
    vector<double> insert_ms;
    vector<double> search_found_ms;
    vector<double> search_notfound_ms;
    vector<double> delete_ms;
};

static vector<int> prepararExistentes(const vector<AirRecord>& records, int Q) {
    vector<int> keys;
    Q = min(Q, (int)records.size());
    keys.reserve(Q);
    for (int i = 0; i < Q; i++) keys.push_back(records[i].uniqueID);
    return keys;
}

static vector<int> prepararInexistentes(const vector<AirRecord>& records, int Q) {
    int maxID = records[0].uniqueID;
    for (const auto& r : records) maxID = max(maxID, r.uniqueID);

    vector<int> keys;
    keys.reserve(Q);
    for (int i = 1; i <= Q; i++) keys.push_back(maxID + i);
    return keys;
}

// ---------- BENCH BT ----------
static TiemposIter benchBT(const vector<AirRecord>& records,
                        const vector<int>& existentes,
                        const vector<int>& inexistentes,
                        int iters) {
    TiemposIter out;
    out.insert_ms.reserve(iters);
    out.search_found_ms.reserve(iters);
    out.search_notfound_ms.reserve(iters);
    out.delete_ms.reserve(iters);

    for (int t = 0; t < iters; t++) {
        BT arbol;

        // Inserción (TODO el CSV)
        auto a = high_resolution_clock::now();
        for (const auto& r : records) arbol.insertar(r);
        auto b = high_resolution_clock::now();
        out.insert_ms.push_back(duration<double, milli>(b - a).count());

        // Búsqueda existentes
        a = high_resolution_clock::now();
        for (int k : existentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_found_ms.push_back(duration<double, milli>(b - a).count());

        // Búsqueda inexistentes
        a = high_resolution_clock::now();
        for (int k : inexistentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_notfound_ms.push_back(duration<double, milli>(b - a).count());

        // Eliminación de elementos encontrados (sin eliminar subárboles)
        a = high_resolution_clock::now();
        for (int k : existentes) arbol.eliminar(k);
        b = high_resolution_clock::now();
        out.delete_ms.push_back(duration<double, milli>(b - a).count());
    }
    return out;
}

// ---------- BENCH BST ----------
static TiemposIter benchBST(const vector<AirRecord>& records,
                            const vector<int>& existentes,
                            const vector<int>& inexistentes,
                            int iters) {
    TiemposIter out;
    out.insert_ms.reserve(iters);
    out.search_found_ms.reserve(iters);
    out.search_notfound_ms.reserve(iters);
    out.delete_ms.reserve(iters);

    for (int t = 0; t < iters; t++) {
        BST arbol;

        auto a = high_resolution_clock::now();
        for (const auto& r : records) arbol.insertar(r);
        auto b = high_resolution_clock::now();
        out.insert_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : existentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_found_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : inexistentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_notfound_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : existentes) arbol.eliminar(k);
        b = high_resolution_clock::now();
        out.delete_ms.push_back(duration<double, milli>(b - a).count());
    }
    return out;
}

// ---------- BENCH AVL ----------
static TiemposIter benchAVL(const vector<AirRecord>& records,
                            const vector<int>& existentes,
                            const vector<int>& inexistentes,
                            int iters) {
    TiemposIter out;
    out.insert_ms.reserve(iters);
    out.search_found_ms.reserve(iters);
    out.search_notfound_ms.reserve(iters);
    out.delete_ms.reserve(iters);

    for (int t = 0; t < iters; t++) {
        AVL arbol;

        auto a = high_resolution_clock::now();
        for (const auto& r : records) arbol.insertar(r);
        auto b = high_resolution_clock::now();
        out.insert_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : existentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_found_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : inexistentes) arbol.buscar(k);
        b = high_resolution_clock::now();
        out.search_notfound_ms.push_back(duration<double, milli>(b - a).count());

        a = high_resolution_clock::now();
        for (int k : existentes) arbol.remove(k);
        b = high_resolution_clock::now();
        out.delete_ms.push_back(duration<double, milli>(b - a).count());
    }
    return out;
}

static void exportPorIteracion(const string& filename,
                            const vector<double>& bt,
                            const vector<double>& bst,
                            const vector<double>& avl) {
    ofstream out(filename);
    out << "Iteracion,BT_ms,BST_ms,AVL_ms\n";
    int iters = (int)bt.size();
    for (int i = 0; i < iters; i++) {
        out << (i + 1) << "," << bt[i] << "," << bst[i] << "," << avl[i] << "\n";
    }
    out.close();
}

int main() {
    vector<AirRecord> records;

    // Leer TODO el CSV una vez (dataset completo)
    if (!loadAirQualityCSV("Air_Quality.csv", records) || records.empty()) {
        cerr << "Error leyendo CSV o viene vacío.\n";
        return 1;
    }

    const int ITERS = 10;
    const int Q = min(1000, (int)records.size()); // cantidad de consultas/borrados

    vector<int> existentes = prepararExistentes(records, Q);
    vector<int> inexistentes = prepararInexistentes(records, Q);

    cout << "Registros cargados: " << records.size() << "\n";
    cout << "Iteraciones por arbol: " << ITERS << "\n";
    cout << "Consultas/borrados por iteracion (Q): " << Q << "\n\n";

    // Medir (10 iteraciones por árbol)
    TiemposIter bt = benchBT(records, existentes, inexistentes, ITERS);
    TiemposIter bst = benchBST(records, existentes, inexistentes, ITERS);
    TiemposIter avl = benchAVL(records, existentes, inexistentes, ITERS);

    // CSV por iteración (para gráfico lineal)
    exportPorIteracion("tiempos_insercion_por_iteracion.csv", bt.insert_ms, bst.insert_ms, avl.insert_ms);
    exportPorIteracion("tiempos_busqueda_existente_por_iteracion.csv", bt.search_found_ms, bst.search_found_ms, avl.search_found_ms);
    exportPorIteracion("tiempos_busqueda_inexistente_por_iteracion.csv", bt.search_notfound_ms, bst.search_notfound_ms, avl.search_notfound_ms);
    exportPorIteracion("tiempos_eliminacion_por_iteracion.csv", bt.delete_ms, bst.delete_ms, avl.delete_ms);

    // CSV de promedios (para gráfico de barras)
    ofstream outAvg("resultados_tiempos.csv");
    outAvg << "Estructura,Insercion_ms,Busqueda_Existente_ms,Busqueda_Inexistente_ms,Eliminacion_ms\n";
    outAvg << "BT,"  << promedio(bt.insert_ms)  << "," << promedio(bt.search_found_ms)  << "," << promedio(bt.search_notfound_ms)  << "," << promedio(bt.delete_ms)  << "\n";
    outAvg << "BST," << promedio(bst.insert_ms) << "," << promedio(bst.search_found_ms) << "," << promedio(bst.search_notfound_ms) << "," << promedio(bst.delete_ms) << "\n";
    outAvg << "AVL," << promedio(avl.insert_ms) << "," << promedio(avl.search_found_ms) << "," << promedio(avl.search_notfound_ms) << "," << promedio(avl.delete_ms) << "\n";
    outAvg.close();

    cout << "Listo CSV generados:\n";
    cout << " - tiempos_insercion_por_iteracion.csv\n";
    cout << " - tiempos_busqueda_existente_por_iteracion.csv\n";
    cout << " - tiempos_busqueda_inexistente_por_iteracion.csv\n";
    cout << " - tiempos_eliminacion_por_iteracion.csv\n";
    cout << " - resultados_tiempos.csv (promedios)\n";

    return 0;
}
