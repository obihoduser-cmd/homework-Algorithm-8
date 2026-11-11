
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
