# homework-Algorithm-8
Институт Информационных Технологий и Кибербезопасности
Дисциплина: Алгоритмы и Структуры данных
Выполнил: Ибрагимов Магомед АБдуллаевич
Группа: ПИЖ-б-о-25-2

Тема: Двоичный поиск. Хеш-таблицы
Цель работы
1. Реализовать алгоритм двоичного (бинарного) поиска в
отсортированном массиве
2. Освоить итеративную и рекурсивную реализации двоичного
поиска
3. Изучить понятие хеш-таблицы, хеш-функции и методы
разрешения коллизий
4. Реализовать хеш-таблицу с методом цепочек и открытой
адресацией
5. Сравнить производительность двоичного поиска и хеш-таблицы

<img width="658" height="178" alt="image" src="https://github.com/user-attachments/assets/820a59ef-5129-43af-b124-c16e85f96fc7" />
<img width="631" height="81" alt="image" src="https://github.com/user-attachments/assets/f931234f-e3a1-405a-ae53-daf19814366a" />
<img width="648" height="153" alt="image" src="https://github.com/user-attachments/assets/c98810d2-7dc8-4c93-be8b-5b611d06e9a0" />
<img width="639" height="81" alt="image" src="https://github.com/user-attachments/assets/9780456e-cca9-477d-9010-3a399ebd0f09" />
<img width="616" height="424" alt="image" src="https://github.com/user-attachments/assets/2649f10f-de0a-4b83-bc9a-85eb5e81818b" />
Рисунки 1, 2, 3, 4, 5 - Условие заданий

```

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// 1. СТРУКТУРА STUDENT

struct Student {
    string name;
    int age;
    double averageGrade;
};

// 2. БИНАРНЫЙ ПОИСК В МАССИВЕ ПО УБЫВАНИЮ

int binarySearchDescending(const int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        }

        // Массив отсортирован по убыванию
        if (arr[mid] < target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}

// 3. ХЕШ-ТАБЛИЦА МЕТОДОМ ЦЕПОЧЕК

class HashTable {
private:
    struct Node {
        int key;
        Student value;
        Node* next;

        Node(int k, const Student& v)
            : key(k), value(v), next(nullptr) {}
    };

    vector<Node*> table;
    int capacity;
    int count;

    int hashFunction(int key) const {
        int index = key % capacity;

        // Обработка отрицательных ключей
        if (index < 0) {
            index += capacity;
        }

        return index;
    }

public:
    HashTable(int size)
        : capacity(size), count(0) {
        if (size <= 0) {
            throw invalid_argument("Размер таблицы должен быть положительным");
        }

        table.resize(capacity, nullptr);
    }

    // Запрещаем копирование таблицы с динамическими узлами
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Освобождение памяти
    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];

            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Вставка элемента
    void insert(int key, const Student& value) {
        int index = hashFunction(key);

        Node* current = table[index];

        // Если ключ уже существует, обновляем значение
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }

            current = current->next;
        }

        // Создаём новый узел и добавляем в начало списка
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;

        count++;
    }

    // Поиск элемента
    const Student* find(int key) const {
        int index = hashFunction(key);

        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &current->value;
            }

            current = current->next;
        }

        return nullptr;
    }

    // Удаление элемента
    bool remove(int key) {
        int index = hashFunction(key);

        Node* current = table[index];
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (previous == nullptr) {
                    table[index] = current->next;
                } else {
                    previous->next = current->next;
                }

                delete current;
                count--;

                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }

    int getSize() const {
        return count;
    }

    // Вывод таблицы
    void print() const {
        for (int i = 0; i < capacity; i++) {
            cout << i << ": ";

            Node* current = table[i];

            while (current != nullptr) {
                cout << "[" << current->key
                     << ", " << current->value.name << "] ";

                current = current->next;
            }

            cout << endl;
        }
    }
};

// 4. СРАВНИТЕЛЬНЫЙ АНАЛИЗ

void performanceTest(int N) {
    const int QUERY_COUNT = 10000;

    // Создаём массив, отсортированный по убыванию
    vector<int> arr(N);

    for (int i = 0; i < N; i++) {
        arr[i] = N - i;
    }

    // Создаём хеш-таблицу
    // Размер таблицы больше количества элементов
    HashTable ht(2 * N + 1);

    for (int i = 1; i <= N; i++) {
        Student student;

        student.name = "Student_" + to_string(i);
        student.age = 18 + (i % 10);
        student.averageGrade = 3.0 + (i % 20) / 10.0;

        ht.insert(i, student);
    }

    // Подготавливаем одинаковые запросы для обоих алгоритмов
    vector<int> queries(QUERY_COUNT);

    for (int i = 0; i < QUERY_COUNT; i++) {
        queries[i] = (i * 7919LL) % N + 1;
    }

    volatile long long result = 0;

    // Замер бинарного поиска
    auto start = high_resolution_clock::now();

    for (int key : queries) {
        int index = binarySearchDescending(arr.data(), N, key);
        result += index;
    }

    auto end = high_resolution_clock::now();

    double binaryTime =
        duration_cast<microseconds>(end - start).count();

    // Замер поиска в хеш-таблице
    start = high_resolution_clock::now();

    for (int key : queries) {
        const Student* student = ht.find(key);

        if (student != nullptr) {
            result += student->age;
        }
    }

    end = high_resolution_clock::now();

    double hashTime =
        duration_cast<microseconds>(end - start).count();

    cout << left << setw(15) << N
         << setw(25) << binaryTime
         << setw(25) << hashTime << endl;
}

// 5. ГЛАВНАЯ ФУНКЦИЯ

int main() {
    setlocale(LC_ALL, "");

    // Часть 1. Проверка бинарного поиска
    cout << "=== БИНАРНЫЙ ПОИСК ПО УБЫВАНИЮ ===\n";

    int arr[] = {20, 17, 14, 11, 8, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    int target = 11;

    int index = binarySearchDescending(arr, n, target);

    if (index != -1) {
        cout << "Элемент " << target
             << " найден. Индекс: " << index << endl;
    } else {
        cout << "Элемент не найден.\n";
    }

    // Часть 2. Проверка хеш-таблицы
    cout << "\n=== ХЕШ-ТАБЛИЦА ===\n";

    HashTable ht(5);

    Student s1 = {"Иванов", 19, 4.5};
    Student s2 = {"Петров", 20, 4.2};
    Student s3 = {"Сидоров", 18, 4.8};

    ht.insert(101, s1);
    ht.insert(102, s2);
    ht.insert(103, s3);

    cout << "Содержимое таблицы:\n";
    ht.print();

    cout << "\nПоиск студента с ключом 102:\n";

    const Student* found = ht.find(102);

    if (found != nullptr) {
        cout << "Имя: " << found->name << endl;
        cout << "Возраст: " << found->age << endl;
        cout << "Средний балл: " << found->averageGrade << endl;
    } else {
        cout << "Студент не найден.\n";
    }

    cout << "\nУдаление студента с ключом 102:\n";

    if (ht.remove(102)) {
        cout << "Студент удалён.\n";
    }

    ht.print();

    // Часть 3. Сравнение производительности
    cout << "\n=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===\n";

    cout << left << setw(15) << "N"
         << setw(25) << "Бинарный поиск (мкс)"
         << setw(25) << "Хеш-таблица (мкс)" << endl;

    cout << string(65, '-') << endl;

    performanceTest(1000);
    performanceTest(10000);
    performanceTest(100000);

    return 0;
}
```

<img width="573" height="521" alt="image" src="https://github.com/user-attachments/assets/56be097b-4b13-47d5-a886-55563e2e038c" />
Рисунок 6 - Вывод

Контрольные вопросы:

1. Какое условие необходимо для применения двоичного поиска?

Массив должен быть отсортирован. Двоичный поиск работает как для сортировки по возрастанию, так и по убыванию, но алгоритм должен учитывать направление сортировки.

2. Какова сложность двоичного поиска? Почему она логарифмическая?
- Лучший случай: \(O(1)\).
- Средний случай: \(O(\log n)\).
- Худший случай: \(O(\log n)\).
Сложность логарифмическая, потому что на каждом шаге область поиска уменьшается в два раза.

3. В чём разница между итеративной и рекурсивной реализацией двоичного поиска?

Итеративная реализация использует цикл while, а рекурсивная — повторные вызовы функции самой себя. Итеративная версия использует \(O(1)\) дополнительной памяти, рекурсивная — \(O(\log n)\) из-за стека вызовов.

4. Чем опасен вариант mid = (left + right) / 2? Как правильно вычислять середину?

При больших значениях left и right их сумма может превысить максимальное значение типа int, что приведёт к переполнению.

Правильный вариант:
int mid = left + (right - left) / 2;

5. Что такое хеш-таблица? Из каких компонентов она состоит?

Хеш-таблица — структура данных, которая хранит пары «ключ — значение» и обеспечивает быстрый поиск, вставку и удаление.

Основные компоненты:
- Хеш-функция — преобразует ключ в индекс.
- Бакет — ячейка массива для хранения элементов.
- Ключ и значение — хранимые данные.
6. Какие требования предъявляются к хеш-функции?

Хеш-функция должна:
- Быть детерминированной: одинаковый ключ всегда даёт одинаковый хеш.
- Равномерно распределять ключи по таблице.
- Быстро вычисляться.
- Минимизировать количество коллизий.
7. Что такое коллизия в хеш-таблице? Как с ней бороться?

Коллизия — ситуация, когда разные ключи получают одинаковый индекс хеш-таблицы.

Основные способы разрешения:
- Метод цепочек — хранение элементов с одинаковым индексом в связанном списке.
- Открытая адресация — поиск другой свободной ячейки таблицы.
8. В чём отличие метода цепочек от открытой адресации?

При методе цепочек элементы с одинаковым хеш-индексом хранятся в связанном списке.

При открытой адресации все элементы хранятся непосредственно в массиве, а при коллизии ищется другая свободная ячейка.

9. Что такое первичная и вторичная кластеризация?
- Первичная кластеризация — образование длинных последовательностей занятых ячеек при линейном пробировании.
- Вторичная кластеризация — возникновение групп ячеек, которые проверяются в одинаковой последовательности из-за совпадения начального хеша.
10. Какова средняя и худшая сложность операций в хеш-таблице?
Операция	Средний случай	Худший случай
Поиск	\(O(1)\)	\(O(n)\)
Вставка	\(O(1)\)	\(O(n)\)
Удаление	\(O(1)\)	\(O(n)\)

Худший случай возникает, когда большое количество ключей попадает в одну цепочку или приходится проверять множество ячеек.

11. Когда эффективнее использовать двоичный поиск, а когда — хеш-таблицу?

Двоичный поиск эффективен, когда данные уже отсортированы и требуется искать элементы по порядку.

Хеш-таблица эффективна, когда необходимо быстро искать значения по ключу, а порядок элементов не имеет значения.

12. Что такое коэффициент загрузки хеш-таблицы? Почему он важен?

Коэффициент загрузки — отношение количества элементов к количеству ячеек таблицы.

13. Как работает рехеширование (динамическое расширение хеш-таблицы)?

Рехеширование — процесс увеличения размера хеш-таблицы при достижении определённого коэффициента загрузки.

Основные этапы:
1. Создаётся новая таблица большего размера.
2. Для всех элементов заново вычисляются хеш-индексы.
3. Элементы переносятся в новую таблицу.
4. Старая таблица освобождается.
14. Как выбрать размер хеш-таблицы? Почему часто выбирают простое число?

Размер таблицы выбирают с учётом предполагаемого количества элементов и коэффициента загрузки.

Часто выбирают простое число, потому что оно помогает равномернее распределять ключи при использовании остатка от деления в хеш-функции и уменьшает влияние некоторых закономерностей в ключах.

15. Как реализовать хеш-функцию для строк?

Один из способов — последовательно обрабатывать символы строки, умножать текущий хеш на некоторое число и прибавлять код очередного символа.
