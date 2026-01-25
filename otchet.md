# Отчёт по лабораторной работе

## Титульный лист

МИНИСТЕРСТВО ОБРАЗОВАНИЯ И НАУКИ РФ  
Национальный исследовательский университет "Высшая школа экономики" 
МИЭМ
Кафедра компьютерной безопасности
Отчёт № _9_
по дисциплине «Языки программирования»  

Тема: «Шаблонные функции»

Выполнил: студент группы _253_  
ФИО Нуралиева Медина Мохмадовна_____________________________________

Проверил: _____________________________



## 1 Цель работы

Изучить применение:

- шаблонных функций и обобщённого программирования в C++;
- стандартных контейнеров `std::vector` и `std::map`;
- лямбда‑выражений;


---

## 2 Постановка задачи

1. Реализовать шаблонную функцию `groupStats`, которая:
   - принимает на вход произвольный контейнер с элементами типа `T`;
   - использует заданную функцию формирования ключа `keyFunc` для отнесения элементов к группам;
   - использует функцию получения значения `valueFunc` для вычисления суммы и среднего;
   - для каждой группы вычисляет:
     - количество элементов;
     - минимальный и максимальный элемент;
     - сумму и среднее значение.

2. Вернуть результат работы функции в виде вектора структур, содержащих рассчитанные характеристики по каждой группе.

3. Продемонстрировать работу функции на примере массива целых чисел.

---

## 3 Теоретические сведения (кратко)

- **Шаблоны функций** позволяют писать обобщённый код, независимый от конкретных типов данных.
- **Стандартный контейнер `std::map`** обеспечивает хранение пар «ключ–значение» с автоматической сортировкой по ключу и быстрым доступом к элементам.
- **Лямбда‑выражения** предоставляют удобный способ описания небольших функций непосредственно в месте вызова.

---

## 4 Описание алгоритма

### 4.1. Структуры данных

Для представления результата по каждой группе используется шаблонная структура:

```cpp
template <typename Key, typename Val, typename Elem>
struct GroupInfo {
    Key   key;       // значение ключа группы
    long  gsize = 0; // количество элементов в группе
    Elem  minEl;     // минимальный элемент
    Elem  maxEl;     // максимальный элемент
    Val   sum;       // сумма значений
    double avg = 0.0;// среднее значение
};
```

Во внутреннем алгоритме группировки применяется вспомогательная структура:

```cpp
struct GroupData {
    long gsize = 0;
    T    minEl{};
    T    maxEl{};
    Val  sum{};
    bool initialized = false;
};
```

`GroupData` хранится в ассоциативном контейнере `std::map<Key, GroupData> groups`, где `Key` — тип ключа группы.

### 4.2. Алгоритм функции `groupStats`

1. Определяются типы:
   - `T` — тип элементов контейнера;
   - `Key` — тип ключа, возвращаемый `keyFunc`;
   - `Val` — тип значения, возвращаемый `valueFunc`.

2. Инициализируется пустая карта `groups` для накопления промежуточных данных по группам и вектор `result` для конечного результата.

3. Для каждого элемента `x` входного контейнера:
   - вычисляется ключ `key = keyFunc(x)`;
   - вычисляется значение `val = valueFunc(x)`;
   - по ключу `key` берётся (или создаётся) запись `GroupData`.

4. Если группа встречается первый раз (`initialized == false`):
   - минимальное и максимальное значения приравниваются к текущему элементу;
   - сумма и размер группы инициализируются значениями текущего элемента;
   - флаг `initialized` устанавливается в `true`.

5. При последующих вхождениях в ту же группу:
   - при необходимости обновляются `minEl` и `maxEl`;
   - к сумме добавляется очередное значение;
   - счётчик элементов группы увеличивается.

6. После обработки всех элементов:
   - вектор результата резервирует память под количество групп;
   - по всем парам `` из `groups` формируются объекты `GroupInfo`, в которых дополнительно вычисляется среднее значение `avg = sum / gsize`.

7. Вектор с заполненными структурами `GroupInfo` возвращается как результат работы функции.

### 4.3. Логика функции `main`

1. Задаётся статический массив целых чисел от 10 до 17.
2. На его основе создаётся динамический контейнер `std::vector<int>`.
3. Вызывается функция `groupStats`, где:
   - в качестве ключа используется остаток от деления на 2 (`x % 2`);
   - в качестве значения — само число (`x`).
4. Полученный вектор групп последовательно выводится на экран: для каждой группы отображаются ключ, размер, минимальное, максимальное значение, сумма и среднее.

---

## 5 Листинг программы

### 5.1. Файл `main.cpp`

```cpp
#include <iostream>
#include <vector>
#include "group_stats.hpp"

int main() {
    int arr = {10, 11, 12, 13, 14, 15, 16, 17};
    std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr));

    // Группировка по остатку от деления на 2, значение – само число
    auto stats = groupStats(vec,
                            (int x) { return x % 2; },
                            (int x) { return x; });

    for (const auto& g : stats) {
        std::cout << "Key: " << g.key
                  << ", Size: " << g.gsize
                  << ", Min: " << g.minEl
                  << ", Max: " << g.maxEl
                  << ", Sum: " << g.sum
                  << ", Avg: " << g.avg
                  << std::endl;
    }

    return 0;
}
```

### 5.2. Файл `group_stats.hpp`

```cpp
#ifndef GROUP_STATS_HPP
#define GROUP_STATS_HPP

#include <vector>
#include <map>
#include <utility>

template <typename Key, typename Val, typename Elem>
struct GroupInfo {
    Key   key;
    long  gsize = 0;
    Elem  minEl;
    Elem  maxEl;
    Val   sum;
    double avg = 0.0;
};

template <typename Container, typename KeyFunc, typename ValueFunc>
auto groupStats(const Container& arr, KeyFunc keyFunc, ValueFunc valueFunc) {
    using T   = typename Container::value_type;
    using Key = decltype(keyFunc(std::declval<T>()));
    using Val = decltype(valueFunc(std::declval<T>()));

    struct GroupData {
        long gsize = 0;
        T    minEl{};
        T    maxEl{};
        Val  sum{};
        bool initialized = false;
    };

    std::map<Key, GroupData> groups;
    std::vector<GroupInfo<Key, Val, T>> result;

    for (const auto& x : arr) {
        Key key = keyFunc(x);
        Val val = valueFunc(x);
        auto& data = groups;

        if (!data.initialized) {
            data.minEl = x;
            data.maxEl = x;
            data.sum   = val;
            data.gsize = 1;
            data.initialized = true;
        } else {
            if (x > data.maxEl) data.maxEl = x;
            if (x < data.minEl) data.minEl = x;
            data.sum += val;
            data.gsize++;
        }
    }

    result.reserve(groups.size());

    for (const auto& [key, data] : groups) {
        GroupInfo<Key, Val, T> group;
        group.key = key;
        group.gsize = data.gsize;
        group.minEl = data.minEl;
        group.maxEl = data.maxEl;
        group.sum = data.sum;
        group.avg = (data.gsize != 0) ? double(data.sum) / data.gsize : 0.0;
        result.push_back(group);
    }

    return result;
}

#endif // GROUP_STATS_HPP
