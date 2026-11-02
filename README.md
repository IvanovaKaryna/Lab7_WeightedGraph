# Lab7_WeightedGraph (варіант 10, «Найкоротші шляхи»)

Статична бібліотека `WeightedGraph` (C++20, CMake, vcpkg, Google Test).
Неорієнтований граф з невід'ємними цілими вагами, алгоритм Дейкстри O(V^2).

## Інваріанти класу WeightedGraph
1. Кількість вершин > 0.
2. Матриця `graph` має розмір V x V.
3. Матриця симетрична: `graph[a][b] == graph[b][a]`.
4. Елемент матриці - `NO_EDGE` (-1, ребра немає) або вага >= 0.

## Ієрархія винятків
```
std::runtime_error
└─ GraphException
   ├─ InvalidWeightException   (parameter, value, minAllowed)
   └─ PathNotFoundException    (start, finish)
```
Стандартні винятки: `std::invalid_argument`, `std::out_of_range`, `std::overflow_error`.

| Операція | Помилка | Виняток |
|---|---|---|
| конструктор | вершин <= 0 | std::invalid_argument |
| усі методи з вершинами | вершина поза [0, V-1] | std::out_of_range |
| addEdge, replaceEdges | вага < 0 | InvalidWeightException |
| shortestPath, reconstructPath | шляху немає | PathNotFoundException |
| shortestPath, pathLength | довжина > INT_MAX | std::overflow_error |
| pathLength | порожній маршрут / немає ребра | std::invalid_argument |

## Гарантії безпеки
| Метод | Гарантія |
|---|---|
| конструктор | при винятку об'єкт не створюється |
| addEdge | strong (перевірки до зміни стану) |
| replaceEdges | strong (тимчасова матриця + swap) |
| hasEdge, shortestPath, reconstructPath, pathLength | const, стан не змінюють |
| vertices, edgeCount, деструктор ScopeLogger | no-throw (noexcept) |

## Розкручування стека (ExceptionDemo)
Очікувана послідовність (записана ДО запуску):
```
[ENTER] level1
[ENTER] level2
[ENTER] level3
level3: file opened, adding an edge with a negative weight...
[EXIT ] level3        <- спочатку знищується файл (ofstream), потім logger level3
[level2] caught InvalidWeightException ...
[EXIT ] level2
[EXIT ] level1
[main] caught InvalidWeightException
```
Фактичний результат збігся з очікуваним: об'єкти знищуються у зворотному
порядку створення, файл `raii_demo.txt` закривається автоматично.

## Некоректні підходи (AntipatternDemo)
`throw 42;` і `throw std::string(...)` не ловляться `catch (const std::exception&)`,
а лише `catch (...)`, де немає ні типу, ні тексту помилки.
Висновок: краще кидати стандартні або власні класи, похідні від `std::exception`:
вони мають `what()`, можуть нести додаткові дані й ловляться одним обробником.

## Збірка і запуск
```
cmake --preset x64-debug
cmake --build out/build/x64-debug
ctest --test-dir out/build/x64-debug --output-on-failure
```
