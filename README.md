# Computational Geometry

**English | [Русская версия](#русская-версия)**

A collection of C++ solutions to algorithmic problems in computational geometry.

The repository covers geometric algorithms in two and three dimensions, spatial data structures, convex polygons, triangulation, and nearest-neighbor search. Each solution is implemented as a standalone program that reads input from standard input and writes results to standard output.

## Implemented algorithms

### Convex geometry
- Convex hull construction using a Graham-scan-style approach
- Convex hull perimeter calculation
- Convex polygon diameter using rotating calipers
- Three-dimensional convex hull construction with incremental face updates
- Minkowski sum of convex polygons
- Point-in-polygon testing

### Triangulation and Voronoi diagrams
- Incremental Delaunay triangulation using orientation and in-circle predicates
- Edge filtering based on geometric conditions
- Voronoi cell construction by successive half-plane clipping
- Calculation of Voronoi cell areas within a rectangular domain

### Spatial data structures
- **KD-tree:** construction using median partitioning with `std::nth_element`; nearest-neighbor queries with branch pruning
- **Quadtree:** spatial partitioning for moving objects, supporting insertion, removal, movement, and spatial interaction queries

## Technologies

- C++
- C++ Standard Library
- STL containers and algorithms

The solutions are standalone programs and do not require third-party geometry libraries.

## Repository structure

| File | Description |
|---|---|
| `tasks/first_task.cpp` | Convex hull perimeter |
| `diameter.cpp` | Convex hull diameter using rotating calipers |
| `star.cpp` | 3D convex hull |
| `point_on_poly.cpp` | Point-in-polygon test |
| `stupid_fight.cpp` | Minkowski sum and polygon intersection-related test |
| `lep.cpp` | Delaunay triangulation and edge filtering |
| `vorona.cpp` | Voronoi cell areas |
| `kd-tree.cpp` | KD-tree and two-nearest-neighbor queries |
| `quadrotree.cpp` | Quadtree for moving objects and spatial interactions |

## Compilation and execution

Each solution contains its own `main()` function and can be compiled independently.

For example:

```bash
g++ -std=c++17 -O2 diameter.cpp -o diameter
./diameter
```

The expected input format depends on the selected problem. Refer to the corresponding source file for the input and output interface.

## Notes

The repository contains solutions to individual algorithmic problems rather than a unified geometry library. Implementations are intended for algorithmic practice and exploration; formal correctness verification and production readiness are not claimed.

---

# Русская версия

**Computational Geometry** — коллекция решений алгоритмических задач по вычислительной геометрии на C++.

В репозитории представлены алгоритмы двумерной и трёхмерной геометрии, пространственные структуры данных, операции над выпуклыми многоугольниками, триангуляция и поиск ближайших соседей.

Каждое решение представляет собой отдельную программу, считывающую входные данные из стандартного ввода и выводящую результат в стандартный вывод.

## Реализованные алгоритмы

### Выпуклая геометрия
- Построение выпуклой оболочки алгоритмом типа сканирования Грэхема
- Вычисление периметра выпуклой оболочки
- Поиск диаметра выпуклого многоугольника методом вращающихся калиперов
- Построение трёхмерной выпуклой оболочки с инкрементальным обновлением граней
- Сумма Минковского выпуклых многоугольников
- Проверка принадлежности точки многоугольнику

### Триангуляция и диаграммы Вороного
- Инкрементальная триангуляция Делоне с использованием геометрических предикатов ориентации и принадлежности описанной окружности
- Фильтрация рёбер по геометрическим условиям
- Построение ячеек Вороного последовательным отсечением полуплоскостями
- Вычисление площадей ячеек Вороного внутри прямоугольной области

### Пространственные структуры данных
- **KD-tree:** построение с разбиением по медиане через `std::nth_element`, поиск двух ближайших соседей с отсечением ветвей
- **Квадродерево (Quadtree):** пространственное разбиение для движущихся объектов, поддержка добавления, удаления, перемещения и запросов пространственного взаимодействия

## Технологии

- C++
- Стандартная библиотека C++
- Контейнеры и алгоритмы STL

Решения не требуют сторонних библиотек вычислительной геометрии.

## Структура репозитория

| Файл | Назначение |
|---|---|
| `tasks/first_task.cpp` | Периметр выпуклой оболочки |
| `diameter.cpp` | Диаметр выпуклой оболочки |
| `star.cpp` | Трёхмерная выпуклая оболочка |
| `point_on_poly.cpp` | Принадлежность точки многоугольнику |
| `stupid_fight.cpp` | Сумма Минковского и геометрическая проверка пересечения |
| `lep.cpp` | Триангуляция Делоне и фильтрация рёбер |
| `vorona.cpp` | Площади ячеек Вороного |
| `kd-tree.cpp` | KD-tree и поиск двух ближайших соседей |
| `quadrotree.cpp` | Квадродерево и взаимодействия движущихся объектов |

## Компиляция и запуск

Каждый файл содержит собственную функцию `main()` и компилируется отдельно.

Например:

```bash
g++ -std=c++17 -O2 diameter.cpp -o diameter
./diameter
```

Формат входных и выходных данных зависит от конкретной задачи и определяется соответствующей реализацией.

## Примечание

Репозиторий содержит самостоятельные решения алгоритмических задач, а не единую библиотеку вычислительной геометрии. Формальная проверка корректности и готовность к промышленному использованию не заявляются.
