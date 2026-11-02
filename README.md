# Lab7_WeightedGraph

**Лабораторна робота №7.** Обробка виключень. Тестування програми.
**Варіант 10:** «Найкоротші шляхи в графі» (клас `WeightedGraph`).
**Виконала:** Іванова Карина, група ФІТ 3-10.

У лабораторній №4 я реалізувала функції `dijkstra()`, `shortestPath()`, `pathLength()`, `reconstructPath()`. У цій роботі вони об'єднані в клас `WeightedGraph`, до якого додано перевірки вхідних даних, власні винятки, гарантії безпеки, демонстрацію RAII та модульні тести.

## Структура проєкту

| Файл | Призначення |
|---|---|
| `Lab7_WeightedGraph.h`, `WeightedGraph.cpp` | клас `WeightedGraph` (статична бібліотека `WeightedGraph`) |
| `GraphExceptions.h` | власна ієрархія винятків |
| `ScopeLogger.h` | RAII-клас для дослідження розкручування стека |
| `Lab7_WeightedGraph.cpp` | демонстраційна програма з обробкою помилкових ситуацій |
| `ExceptionDemo.cpp` | демонстрація RAII та розкручування стека |
| `AntipatternDemo.cpp` | демонстрація некоректних підходів (`throw int`, `throw std::string`) |
| `WeightedGraphTests.cpp` | модульні тести Google Test |
| `CMakeLists.txt`, `CMakePresets.json`, `vcpkg.json` | збирання та залежності |

## Збирання та запуск

1. Visual Studio, проєкт CMake, конфігурація `x64 Debug`. Google Test підключається через vcpkg (manifest-режим, файл `vcpkg.json`).
2. Тести з термінала:
   ```
   ctest --test-dir out/build/x64-debug --output-on-failure
   ```
   або запуск `WeightedGraphTests.exe` напряму.
3. Демонстрації: `Lab7_WeightedGraph.exe`, `ExceptionDemo.exe`, `AntipatternDemo.exe`.

## Інваріанти класу `WeightedGraph`

1. Кількість вершин `vertexCount > 0` (перевіряється в конструкторі).
2. Матриця суміжності `graph` квадратна: `vertexCount x vertexCount`.
3. Матриця симетрична: `graph[u][v] == graph[v][u]` (граф неорієнтований).
4. Кожен елемент матриці дорівнює `NO_EDGE` (ребра немає) або є вагою `>= 0`.

Усі перевірки виконуються до зміни даних, тому некоректний стан недоступний через публічний інтерфейс.

## Ієрархія винятків

```
std::exception
 └── std::runtime_error
      └── GraphException                  (базовий клас бібліотеки)
           ├── InvalidWeightException     (parameter(), value(), minAllowed())
           └── PathNotFoundException      (start(), finish())
```

Зі стандартних винятків використано `std::invalid_argument`, `std::out_of_range` і `std::overflow_error`.

| Операція | Умова помилки | Виняток |
|---|---|---|
| `WeightedGraph(n)` | `n <= 0` | `std::invalid_argument` |
| `addEdge`, `replaceEdges`, `hasEdge` | вершина поза `[0, n-1]` | `std::out_of_range` |
| `addEdge`, `replaceEdges` | від'ємна вага (для Дейкстри недопустима) | `InvalidWeightException` |
| `shortestPath`, `reconstructPath` | початкова або кінцева вершина недопустима | `std::out_of_range` |
| `shortestPath`, `reconstructPath` | шляху між вершинами не існує | `PathNotFoundException` |
| `shortestPath`, `pathLength` | довжина шляху більша за `INT_MAX` | `std::overflow_error` |
| `pathLength` | недопустима вершина в маршруті | `std::out_of_range` |
| `pathLength` | порожній маршрут або немає ребра між сусідніми вершинами | `std::invalid_argument` |

Усі блоки `catch` ловлять виняток за `const&` і розташовані від конкретного типу до загального. Повторне генерування виконується через `throw;`.

## Гарантії безпеки щодо винятків

| Метод | Гарантія | Як досягається |
|---|---|---|
| `addEdge` | strong | усі перевірки до зміни стану, далі присвоєння `int` (винятків не кидають) |
| `replaceEdges` | strong | prepare-then-commit: нова матриця будується в тимчасовому об'єкті, `swap()` виконується після успіху |
| `vertices`, `edgeCount` | no-throw (`noexcept`) | лише читання |
| `shortestPath`, `reconstructPath`, `pathLength`, `hasEdge` | стан не змінюють | `const`-методи |
| `~ScopeLogger` | no-throw (`noexcept`) | деструктор не випускає винятків |

## Дослідження розкручування стека

Очікувана послідовність (записана **до** запуску `ExceptionDemo`):

```
RAII AND EXCEPTIONS DEMONSTRATION
[ENTER] level1
[ENTER] level2
[ENTER] level3
level3: file opened, adding an edge with a negative weight...
[EXIT ] level3
[level2] caught InvalidWeightException: parameter=weight, value=-5, minAllowed=0
[EXIT ] level2
[EXIT ] level1
[main] caught InvalidWeightException
[main] what(): Invalid weight: parameter 'weight' = -5, minimum allowed = 0
File content after unwinding: RAII demo: stream closed during unwinding
Program continues normally.
DEMONSTRATION FINISHED
```

Обґрунтування: конструктори `ScopeLogger` викликаються від `level1` до `level3`. Після винятку в `level3` локальні об'єкти знищуються в порядку, зворотному до створення: спочатку граф і файловий потік (потік закривається, дані зберігаються), потім `logger` третього рівня. Обробник у `level2` виводить діагностику й повторно кидає виняток через `throw;`, тип зберігається. Далі знищуються `logger` другого та першого рівнів, і виняток ловить `main`.

**Фактичний результат:** повністю збігся з очікуваним (запуск у Visual Studio, див. скріншот у звіті).

## Некоректні підходи до винятків

Фактичний вивід `AntipatternDemo`:

```
ANTIPATTERN DEMONSTRATION
int was NOT caught by catch (const std::exception&), but was caught by catch (...)
std::string was NOT caught by catch (const std::exception&), but was caught by catch (...)
catch (...) has no parameter, so we cannot read the error text.
DEMONSTRATION FINISHED
```

`throw 42;` і `throw std::string(...)` не перехоплюються обробником `catch (const std::exception&)`. Їх ловить лише `catch (...)`, але він не дає ні типу, ні тексту помилки. Тому слід кидати стандартні або власні класи, похідні від `std::exception`: вони мають єдиний метод `what()`, дозволяють централізовану обробку через базовий клас, будують ієрархію помилок і передають додаткові дані. Повторно кидати виняток треба через `throw;`, а не `throw ex;`: другий варіант копіює лише базову частину об'єкта (object slicing).

## Модульні тести

Google Test, **28 тестів** у п'яти наборах:

| Набір | Кількість | Що перевіряє |
|---|---|---|
| `WeightedGraphTests` | 14 | основні операції, сценарії помилок, діагностичні дані винятків |
| `ExceptionHierarchyTests` | 1 | власні винятки успадковані від `std::runtime_error` |
| `BoundaryTests` | 5 | граничні значення (нульова вага, `INT_MAX`, один вузол, остання вершина) |
| `InvariantTests` | 2 | симетрія матриці, кількість вершин після заміни ребер |
| `StrongGuaranteeTests` | 6 | стан графа до й після невдалої операції |

Ваги ребер цілі, тому результати порівнюються через `EXPECT_EQ`. `EXPECT_NEAR` потрібен лише для чисел із плаваючою комою, а їх у цій бібліотеці немає.

**Результат:** `100% tests passed out of 28`.

## Навмисна помилка (негативний самоконтроль)

Щоб переконатися, що тести справді виявляють помилки, я тимчасово зламала `replaceEdges()`: граф очищується до перевірки нового набору ребер, а нові ребра записуються прямо в `graph`, без тимчасової матриці та `swap()`.

Результат: **25 тестів пройшли, 3 впали**:

- `StrongGuaranteeTests.ReplaceEdgesInvalidVertex`
- `StrongGuaranteeTests.ReplaceEdgesNegativeWeight`
- `StrongGuaranteeTests.ReplaceEdgesPreservesWholeGraph`

Перевірка `EXPECT_THROW` у них проходить, бо виняток справді виникає. Падають перевірки стану: граф після помилки вже змінений. Отже, лише тип винятку не доводить безпеки, потрібно перевіряти й стан об'єкта. Після цього правильну реалізацію (prepare-then-commit) відновлено, і всі 28 тестів знову проходять.
