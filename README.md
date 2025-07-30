# Trabajo Práctico Nº3: Diccionario con Tablas Hash

## Introducción

Este trabajo práctico implementa un diccionario utilizando una **estructura de datos basada en tablas de hashing** para lograr un acceso eficiente a los elementos. Se emplea el método de **encadenamiento (hashing abierto)** para gestionar las colisiones, lo que garantiza un rendimiento estable incluso en situaciones de alta carga.

---

## 🧩 Características Principales

- **Tablas Hash Dinámicas**: El tamaño del diccionario se ajusta automáticamente según la carga actual y los parámetros definidos.
- **Rehashing Automático**: Cuando la capacidad alcanza un umbral predefinido, se realiza automáticamente un rehashing para mantener un factor de carga óptimo.
- **Resolución de Colisiones por Encadenamiento**: Se utilizan listas enlazadas para manejar colisiones, permitiendo el almacenamiento de múltiples claves que comparten el mismo índice.
- **Operaciones Básicas Soportadas**:
  - Inserción
  - Eliminación
  - Búsqueda
  - Verificación de existencia
  - Recuperación de valor
  - Obtención del tamaño

---

## ⚙️ Funciones Principales

- `dictionary_create`: Crea un diccionario vacío con capacidad inicial definida.
- `dictionary_put`: Inserta un nuevo par clave-valor.
- `dictionary_get`: Recupera el valor asociado a una clave.
- `dictionary_delete`: Elimina un elemento por su clave.
- `dictionary_contains`: Verifica si una clave existe en el diccionario.
- `dictionary_size`: Devuelve la cantidad actual de elementos.
- `dictionary_destroy`: Libera la memoria utilizada por el diccionario.

---

## 🛠️ Consideraciones de Implementación

- **Parámetros Globales Configurables**: Como el tamaño inicial y el factor de carga, permiten ajustar el comportamiento y rendimiento.
- **Función de Hashing FNV-1a**: Se utiliza para calcular eficientemente el índice de cada clave.
- **Gestión de Memoria**: Incluye chequeo de errores y liberación segura de memoria para evitar fugas y comportamientos inesperados.

---

## 📁 Estructura del Repositorio
├── Dockerfile
├── Makefile
├── tp3.h # Definiciones de estructuras y funciones públicas
├── tp3.c # Implementación de las funciones del diccionario
├── tests.c # Pruebas unitarias del diccionario
├── test_malloc.h # Simulación de malloc y free para pruebas
└── test_malloc.c # Verificaciones de gestión de memoria


- **Dockerfile y Makefile**: Permiten crear un entorno de desarrollo reproducible. Garantizan que el código se ejecute de la misma manera en diferentes sistemas operativos.

- **tp3.h / tp3.c**: Contienen las definiciones y la implementación del diccionario.

- **test_malloc.[h|c]**: Simulan el comportamiento de `malloc` y `free` para verificar la gestión de memoria.

- **tests.c**: Incluye casos de prueba para evaluar:
  - Manejo de colisiones
  - Rehashing automático
  - Gestión adecuada de memoria

---

## 🧪 Cómo correr las pruebas

```bash
make test

- **Dockerfile y Makefile**: Permiten crear un entorno de desarrollo reproducible. Garantizan que el código se ejecute de la misma manera en diferentes sistemas operativos.

- **tp3.h / tp3.c**: Contienen las definiciones y la implementación del diccionario.

- **test_malloc.[h|c]**: Simulan el comportamiento de `malloc` y `free` para verificar la gestión de memoria.

- **tests.c**: Incluye casos de prueba para evaluar:
  - Manejo de colisiones
  - Rehashing automático
  - Gestión adecuada de memoria

---

## 🧪 Cómo correr las pruebas

```bash
make test
