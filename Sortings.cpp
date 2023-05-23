#include "Lib.h"

void Sortirovki::BubleSort(double* a, int size)
{
    //проверка на отсортированность
    bool flag = true;

    //начало сортировки
    while (flag)
    { 
        flag = false;

        // сортировка подмассива с четными индексами
        for (int j = 1; j < size; j += 2)
            if (a[j - 1] > a[j]) //проверка на нарушение порядка и последующая замена с изменением флага
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = true;
            }

        // сортировка подмассива с нечетными индексами
        for (int j = 2; j < size; j += 2)
            if (a[j - 1] > a[j]) //проверка на нарушение порядка и последующая замена с изменением флага
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = true;
            }
    }

}

void Sortirovki::BubleSort_parallel(double* a, int size)
{
    //проверка на отсортированность
    bool flag = false;

    //начало сортировки
    while (!flag)
    {
        flag = true;

        // сортировка подмассива с четными индексами, разделенная по потокам
#pragma omp parallel for shared(a, size) reduction(&&: flag)
        for (int j = 1; j < size; j += 2)
            if (a[j - 1] > a[j]) //проверка на нарушение порядка и последующая замена с изменением флага
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = false;
            }

        // сортировка подмассива с нечетными индексами, разделенная по потокам
#pragma omp parallel for shared(a, size) reduction(&&: flag)
        for (int j = 2; j < size; j += 2)
            if (a[j - 1] > a[j]) //проверка на нарушение порядка и последующая замена с изменением флага
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = false;
            }

#pragma omp barrier // ожидание завершения всех потоков

    }

}

void Sortirovki::FastSort(double* a, size_t first, size_t last, unsigned d) {

    // создание вспомогательных эл-ов - среднего эл-та массива и границ сортируемого массива
    double mid;
    size_t f = first, l = last;
    mid = a[(f + l) / 2];

    // старт сортировки
    while (1) {

        while (a[f] < mid) { ++f; }; // пока порядок соблюдается, сдвигаем левую границу вправо
        while (a[l] > mid) { --l; }; // пока порядок соблюдается, сдвигаем правую границу влево

        if (f >= l) { break; } // проверка на соблюдение порядка всеми эл-ми

        std::swap(a[f++], a[l--]); // замена эл-ов при его несоблюдении

    }

    size_t part = l;

    // рекурсивный вызов для правой и левой частей массива при необходимости
    if (first < part)
        FastSort(a, first, part, d + 1); // сортировка левого подмассива

    if (part + 1 < last)
        FastSort(a, part + 1, last, d + 1); // сортировка правого подмассива

}


void Sortirovki::FastSort_parallel(double* a, size_t first, size_t last, unsigned d) {

    if (last - first > 50)
    {
        // создание вспомогательных эл-ов - среднего эл-та массива и границ сортируемого массива
        double mid;
        size_t f = first, l = last;
        mid = a[(f + l) / 2];

        // старт сортировки
        while (1) {

            while (a[f] < mid) { ++f; }; // пока порядок соблюдается, сдвигаем левую границу вправо
            while (a[l] > mid) { --l; }; // пока порядок соблюдается, сдвигаем правую границу влево

            if (f >= l) { break; } // проверка на соблюдение порядка всеми эл-ми

            std::swap(a[f++], a[l--]); // замена эл-ов при его несоблюдении

        }

        size_t part = l;

        // рекурсивный вызов для правой и левой частей массива при необходимости, разбирый на потоки
#pragma omp parallel
#pragma omp sections
        {
#pragma omp section // сортировка левого подмассива
            {
                if (first < part)
                    FastSort_parallel(a, first, part, d + 1);
            }
#pragma omp section // сортировка правого подмассива
            {
                if (part + 1 < last)
                    FastSort_parallel(a, part + 1, last, d + 1);
            }
        }
    }
    else
    {

        Sortirovki::BubleSort(a + first, last - first + 1);

    }
}


void merge(double *arr, int l, int m, int r) // вспомогательная функция для TimeSort - слияние отсортированных подмассивов
{
    // создание вспомогательных эл-ов - двух подмассивов (правого и левого) и их длин
    int len1 = m - l + 1, len2 = r - m;
    double *left = new double[len1], *right = new double[len2];

    // заполнение подмассивов соответствующими эл-ми
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    // создание вспомогательных эл-ов - счетчиков для левого, правого подмассивов и основного массива
    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2)
    {
        // присвоение наименьшего эл-та и сдвиг соответствующего счетчика
        if (left[i] <= right[j]) // левого
        {

            arr[k] = left[i];
            i++;

        }
        else // правого
        {

            arr[k] = right[j];
            j++;

        }
        k++; // прибавление к основному счетчику
    }

    // заполнение левым подмассивом, если в нем остались эл-ты
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }

    // заполнение правым подмассивом, если в нем остались эл-ты
    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
    }

    // удаление вспомогательных массивов
    delete[] left;
    delete[] right;

}

void Sortirovki::TimeSort(double* a, int size)
{

    // проверяем размер сортируемого массива
    if (size > 10) 
    {

        // рекурсивные вызовы для двух половин массива
        TimeSort(a, size / 2); // левая
        TimeSort(&a[size / 2], size / 2 + size % 2); // правая

        // слияние 
        merge(a, 0, size/2 - 1, size - 1);

    }
    // если размер массива невелик, сортируем его более эффективным способом
    else
    {

        Sortirovki::BubleSort(a, size);

    }

}

void Sortirovki::TimeSort_parallel(double* a, int size)
{

    // проверяем размер сортируемого массива
    if (size > 60)
    {
       
        // рекурсивные вызовы для двух половин массива, разделенные на потоки
#pragma omp parallel
#pragma omp sections
        {

#pragma omp section // левая
            {

                TimeSort_parallel(a, size / 2);

            }
            
#pragma omp section //правая
            {

                TimeSort_parallel(&a[size / 2], size / 2 + size % 2);

            }
            
        }

        // слияние подмассивов
       merge(a, 0, size / 2 - 1, size - 1);

    }
    // если размер массива невелик, сортируем его более эффективным способом
    else
    {

        Sortirovki::BubleSort(a, size);

    }

}

bool Sortirovki::CheckArray(double* a, int size) //ф-я проверки массива на отсортированность
{

    for (int i = 0; i < size - 1; i++) if (a[i] > a[i + 1]) return false; // при нахождении неотсортированных 2-х эл-ов возвращает False
    return true; // иначе True

}