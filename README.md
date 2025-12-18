# 📘 Proyecto Semestral – Estructuras de Datos  
**Comparación de Árbol Binario (BT), BST y AVL**

---

## 📌 Descripción del Proyecto

Este proyecto tiene como objetivo **comparar el rendimiento** de tres estructuras de datos basadas en árboles:

- **BT**: Árbol Binario Simple  
- **BST**: Árbol Binario de Búsqueda  
- **AVL**: Árbol Binario Balanceado  

Las comparaciones se realizan sobre un dataset real de **calidad del aire de la ciudad de New York**, utilizando como clave el **Unique ID** de cada registro.

Se miden los tiempos promedio de:

- Inserción  
- Búsqueda de elementos existentes  
- Búsqueda de elementos inexistentes  
- Eliminación de elementos  

---

## 📂 Estructura del Proyecto

```
ProyectoED/
│
├── Air_Quality.csv
├── AirRecord.h
├── Lector_CSV.h
│
├── BT.h
├── BST.h
├── AVL.h
│
├── Main.cpp
├── MainBenchmark.cpp
│
├── resultados_tiempos.csv
├── graficos.py
│
└── README.md
```

---

## ⚙️ Requisitos

- **C++ (C++17)**
- Compilador `g++`
- **Python 3**
- Librería `matplotlib`

---

## ▶️ Compilar y Ejecutar

### 🧪 Programa de pruebas (`Main.cpp`)

```bash
g++ -std=c++17 Main.cpp -O2 -o ProyectoED
```

```bash
ProyectoED.exe
```

---

### ⏱️ Programa de Benchmark (`MainBenchmark.cpp`)

```bash
g++ -std=c++17 MainBenchmark.cpp -O2 -o BenchmarkED
```

```bash
BenchmarkED.exe
```

Esto genera el archivo:

```
resultados_tiempos.csv
```

---

## 📊 Generar Gráficos

Instalar matplotlib si es necesario:

```bash
pip install matplotlib
```

Ejecutar:

```bash
python graficos.py
```

Se generan:

- `grafico_lineal.png`
- `grafico_barras.png`

---

## 📈 Resumen de Resultados

- **BT**: Peor rendimiento (operaciones O(n))
- **BST**: Buen rendimiento, depende del orden de inserción
- **AVL**: Inserción un poco más lenta, pero búsquedas más rápidas y estables

---

## 👥 Autores

- Integrante 1: Renato  Quezada
- Integrante 2: Martina Concha

Asignatura: **Estructuras de Datos**

---

## ✅ Notas Finales

- Proyecto desarrollado en **C++**
- Medición de tiempos con `std::chrono`
- Resultados analizados de forma teórica y experimental
