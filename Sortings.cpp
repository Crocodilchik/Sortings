#include "Lib.h"

void Sortirovki::BubleSort(double* a, int size)
{
    //�������� �� �����������������
    bool flag = true;

    //������ ����������
    while (flag)
    { 
        flag = false;

        // ���������� ���������� � ������� ���������
        for (int j = 1; j < size; j += 2)
            if (a[j - 1] > a[j]) //�������� �� ��������� ������� � ����������� ������ � ���������� �����
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = true;
            }

        // ���������� ���������� � ��������� ���������
        for (int j = 2; j < size; j += 2)
            if (a[j - 1] > a[j]) //�������� �� ��������� ������� � ����������� ������ � ���������� �����
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
    //�������� �� �����������������
    bool flag = false;

    //������ ����������
    while (!flag)
    {
        flag = true;

        // ���������� ���������� � ������� ���������, ����������� �� �������
#pragma omp parallel for shared(a, size) reduction(&&: flag)
        for (int j = 1; j < size; j += 2)
            if (a[j - 1] > a[j]) //�������� �� ��������� ������� � ����������� ������ � ���������� �����
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = false;
            }

        // ���������� ���������� � ��������� ���������, ����������� �� �������
#pragma omp parallel for shared(a, size) reduction(&&: flag)
        for (int j = 2; j < size; j += 2)
            if (a[j - 1] > a[j]) //�������� �� ��������� ������� � ����������� ������ � ���������� �����
            {
                double v = a[j - 1];
                a[j - 1] = a[j];
                a[j] = v;
                flag = false;
            }

#pragma omp barrier // �������� ���������� ���� �������

    }

}

void Sortirovki::FastSort(double* a, size_t first, size_t last, unsigned d) {

    // �������� ��������������� ��-�� - �������� ��-�� ������� � ������ ������������ �������
    double mid;
    size_t f = first, l = last;
    mid = a[(f + l) / 2];

    // ����� ����������
    while (1) {

        while (a[f] < mid) { ++f; }; // ���� ������� �����������, �������� ����� ������� ������
        while (a[l] > mid) { --l; }; // ���� ������� �����������, �������� ������ ������� �����

        if (f >= l) { break; } // �������� �� ���������� ������� ����� ��-��

        std::swap(a[f++], a[l--]); // ������ ��-�� ��� ��� ������������

    }

    size_t part = l;

    // ����������� ����� ��� ������ � ����� ������ ������� ��� �������������
    if (first < part)
        FastSort(a, first, part, d + 1); // ���������� ������ ����������

    if (part + 1 < last)
        FastSort(a, part + 1, last, d + 1); // ���������� ������� ����������

}


void Sortirovki::FastSort_parallel(double* a, size_t first, size_t last, unsigned d) {

    if (last - first > 50)
    {
        // �������� ��������������� ��-�� - �������� ��-�� ������� � ������ ������������ �������
        double mid;
        size_t f = first, l = last;
        mid = a[(f + l) / 2];

        // ����� ����������
        while (1) {

            while (a[f] < mid) { ++f; }; // ���� ������� �����������, �������� ����� ������� ������
            while (a[l] > mid) { --l; }; // ���� ������� �����������, �������� ������ ������� �����

            if (f >= l) { break; } // �������� �� ���������� ������� ����� ��-��

            std::swap(a[f++], a[l--]); // ������ ��-�� ��� ��� ������������

        }

        size_t part = l;

        // ����������� ����� ��� ������ � ����� ������ ������� ��� �������������, �������� �� ������
#pragma omp parallel
#pragma omp sections
        {
#pragma omp section // ���������� ������ ����������
            {
                if (first < part)
                    FastSort_parallel(a, first, part, d + 1);
            }
#pragma omp section // ���������� ������� ����������
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


void merge(double *arr, int l, int m, int r) // ��������������� ������� ��� TimeSort - ������� ��������������� �����������
{
    // �������� ��������������� ��-�� - ���� ����������� (������� � ������) � �� ����
    int len1 = m - l + 1, len2 = r - m;
    double *left = new double[len1], *right = new double[len2];

    // ���������� ����������� ���������������� ��-��
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    // �������� ��������������� ��-�� - ��������� ��� ������, ������� ����������� � ��������� �������
    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2)
    {
        // ���������� ����������� ��-�� � ����� ���������������� ��������
        if (left[i] <= right[j]) // ������
        {

            arr[k] = left[i];
            i++;

        }
        else // �������
        {

            arr[k] = right[j];
            j++;

        }
        k++; // ����������� � ��������� ��������
    }

    // ���������� ����� �����������, ���� � ��� �������� ��-��
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }

    // ���������� ������ �����������, ���� � ��� �������� ��-��
    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
    }

    // �������� ��������������� ��������
    delete[] left;
    delete[] right;

}

void Sortirovki::TimeSort(double* a, int size)
{

    // ��������� ������ ������������ �������
    if (size > 10) 
    {

        // ����������� ������ ��� ���� ������� �������
        TimeSort(a, size / 2); // �����
        TimeSort(&a[size / 2], size / 2 + size % 2); // ������

        // ������� 
        merge(a, 0, size/2 - 1, size - 1);

    }
    // ���� ������ ������� �������, ��������� ��� ����� ����������� ��������
    else
    {

        Sortirovki::BubleSort(a, size);

    }

}

void Sortirovki::TimeSort_parallel(double* a, int size)
{

    // ��������� ������ ������������ �������
    if (size > 60)
    {
       
        // ����������� ������ ��� ���� ������� �������, ����������� �� ������
#pragma omp parallel
#pragma omp sections
        {

#pragma omp section // �����
            {

                TimeSort_parallel(a, size / 2);

            }
            
#pragma omp section //������
            {

                TimeSort_parallel(&a[size / 2], size / 2 + size % 2);

            }
            
        }

        // ������� �����������
       merge(a, 0, size / 2 - 1, size - 1);

    }
    // ���� ������ ������� �������, ��������� ��� ����� ����������� ��������
    else
    {

        Sortirovki::BubleSort(a, size);

    }

}

bool Sortirovki::CheckArray(double* a, int size) //�-� �������� ������� �� �����������������
{

    for (int i = 0; i < size - 1; i++) if (a[i] > a[i + 1]) return false; // ��� ���������� ����������������� 2-� ��-�� ���������� False
    return true; // ����� True

}