#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <iomanip>

using namespace std;

// пространство имен сортировок
namespace Sortirovki
{
	// сортировка пузырьком
	void BubleSort(double* a, int size);
	void BubleSort_parallel(double* a, int size);

	// сортировка Хоара
	void FastSort(double* a, size_t first, size_t last, unsigned d = 0);
	void FastSort_parallel(double* a, size_t first, size_t last, unsigned d = 0);

	// сортировка слиянием + пузырьком
	void TimeSort(double* a, int size);
	void TimeSort_parallel(double* a, int size);

	// проверка на отсортированность
	bool CheckArray(double* a, int size);

}

void merge(double* arr, int l, int m, int r);