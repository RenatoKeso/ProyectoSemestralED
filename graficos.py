import csv
from pathlib import Path
import matplotlib.pyplot as plt

# CSVs por iteración (10 iteraciones)
CSV_LINEALES = [
    ("tiempos_insercion_por_iteracion.csv", "Inserción", "grafico_lineal_insercion.png"),
    ("tiempos_busqueda_existente_por_iteracion.csv", "Búsqueda Existente", "grafico_lineal_busqueda_existente.png"),
    ("tiempos_busqueda_inexistente_por_iteracion.csv", "Búsqueda Inexistente", "grafico_lineal_busqueda_inexistente.png"),
    ("tiempos_eliminacion_por_iteracion.csv", "Eliminación", "grafico_lineal_eliminacion.png"),
]

# CSV de promedios (para barras)
CSV_PROMEDIOS = "resultados_tiempos.csv"
OUT_BARRAS = "grafico_barras_promedios.png"


def leer_csv_iteraciones(path_csv):
    """
    Lee un CSV con formato:
    Iteracion,BT_ms,BST_ms,AVL_ms
    1,....
    """
    iters = []
    bt = []
    bst = []
    avl = []

    with open(path_csv, newline="", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            iters.append(int(row["Iteracion"]))
            bt.append(float(row["BT_ms"]))
            bst.append(float(row["BST_ms"]))
            avl.append(float(row["AVL_ms"]))

    return iters, bt, bst, avl


def graficar_lineal(csv_file, titulo_operacion, out_png):
    iters, bt, bst, avl = leer_csv_iteraciones(csv_file)

    plt.figure()
    plt.plot(iters, bt, marker="o", label="BT")
    plt.plot(iters, bst, marker="o", label="BST")
    plt.plot(iters, avl, marker="o", label="AVL")

    plt.xlabel("Iteración")
    plt.ylabel("Tiempo (ms)")
    plt.title(f"Tiempos por iteración - {titulo_operacion}")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)


def leer_promedios(csv_file):
    """
    Lee resultados_tiempos.csv con formato:
    Estructura,Insercion_ms,Busqueda_Existente_ms,Busqueda_Inexistente_ms,Eliminacion_ms
    BT,....
    """
    filas = []
    with open(csv_file, newline="", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            filas.append({
                "Estructura": row["Estructura"],
                "Insercion_ms": float(row["Insercion_ms"]),
                "Busqueda_Existente_ms": float(row["Busqueda_Existente_ms"]),
                "Busqueda_Inexistente_ms": float(row["Busqueda_Inexistente_ms"]),
                "Eliminacion_ms": float(row["Eliminacion_ms"]),
            })
    return filas


def graficar_barras_promedios(csv_file, out_png):
    data = leer_promedios(csv_file)
    if not data:
        print("CSV de promedios vacío.")
        return

    # Orden fijo
    orden = ["BT", "BST", "AVL"]
    data.sort(key=lambda x: orden.index(x["Estructura"]) if x["Estructura"] in orden else 99)

    ops = ["Inserción", "Búsq. Existente", "Búsq. Inexistente", "Eliminación"]
    keys = ["Insercion_ms", "Busqueda_Existente_ms", "Busqueda_Inexistente_ms", "Eliminacion_ms"]

    x = list(range(len(ops)))
    width = 0.25

    plt.figure()
    for i, row in enumerate(data):
        y = [row[k] for k in keys]
        xi = [v + i * width for v in x]
        plt.bar(xi, y, width=width, label=row["Estructura"])

    # Centrar los ticks (3 barras)
    centro = [v + width for v in x]
    plt.xticks(centro, ops)
    plt.ylabel("Tiempo promedio (ms)")
    plt.title("Comparación de promedios por operación (BT vs BST vs AVL)")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)


def main():
    # Verificar archivos
    faltan = []
    for csv_name, _, _ in CSV_LINEALES:
        if not Path(csv_name).exists():
            faltan.append(csv_name)
    if not Path(CSV_PROMEDIOS).exists():
        faltan.append(CSV_PROMEDIOS)

    if faltan:
        print("Faltan estos archivos CSV en la carpeta:")
        for f in faltan:
            print(" -", f)
        print("\nCorre primero el Benchmark (MainBenchmark.cpp) para generarlos.")
        return

    # Graficos lineales
    for csv_name, titulo, out_png in CSV_LINEALES:
        graficar_lineal(csv_name, titulo, out_png)

    # Barras de promedios
    graficar_barras_promedios(CSV_PROMEDIOS, OUT_BARRAS)

    print(" Gráficos generados:")
    for _, _, out_png in CSV_LINEALES:
        print(" -", out_png)
    print(" -", OUT_BARRAS)

    plt.show()


if __name__ == "__main__":
    main()
