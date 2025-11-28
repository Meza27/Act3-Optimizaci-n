# 🧮 Expr Calc + Optimización con LLVM

Este proyecto consiste en la implementación de una calculadora de expresiones aritméticas utilizando **ANTLR4** para la fase de análisis sintáctico y **LLVM** para la generación y optimización de código intermedio. A partir de archivos `.expr`, el sistema genera código LLVM **sin optimizar** (`sinopt.ll`) y **optimizado** (`conopt.ll`), permitiendo comparar de forma clara los efectos de la optimización. ⚙️  

El proyecto se desarrolló en el contexto del curso **Teoría de Compiladores**.

---

## ✨ Funcionalidades
- Lectura de expresiones desde archivos `.expr`
- Generación de código LLVM sin optimizar (`sinopt.ll`)
- Generación de código LLVM optimizado (`conopt.ll`)
- Soporte para operaciones aritméticas
- Uso de variables en las expresiones
- Comparación directa entre código optimizado y no optimizado

---

## ✅ Requisitos
- ANTLR 4
- LLVM
- CMake
- Compilador C++ (g++ o clang)

---

## 🧱 Compilación
```bash
mkdir build
cd build
cmake ..
make
```
---

## ▶️ Ejecución
./Calc ../../ejemplo.expr
./Calc ../../ejemplo.expr -O
./Calc ../../ejemplo.expr -jit

---

## 📈 Resultados
Al ejecutar los ejemplos propuestos, se pudo observar claramente el impacto de la optimización. En el código sin optimizar, se mantienen llamadas completas, saltos innecesarios entre bloques y estructuras más extensas. En cambio, en el código optimizado, LLVM elimina instrucciones redundantes, simplifica el flujo del programa y añade atributos que mejoran el rendimiento del ejecutable.

Esto evidencia cómo el proceso de optimización permite obtener un código más limpio, eficiente y adecuado para una ejecución real.


