#include "Lib.h"
#include <utility>

constexpr auto SIZE = 1000000;

int main()
{
	//запуск генератора случайных чисел
	srand(time(NULL));

	//заголовок таблицы
	cout << setw(20) << "Sorting" << setw(15) << " Time" << setw(15) << "Is sorted" << endl;
	cout << endl;

	//инициализация и заполнение массивов
	double* a_fst = new double[SIZE];
	double* b_fst = new double[SIZE];

	for (int i = 0; i < SIZE; i++)
	{

		a_fst[i] = rand() * 0.001;
		b_fst[i] = a_fst[i];

	}

	// tstart1 - начальная точка таймера
	//tend1 - конечная точка таймера

	// Buble sort
	double tstart1 = omp_get_wtime();
	Sortirovki::BubleSort(a_fst, SIZE);
	double tend1 = omp_get_wtime();
	cout << setw(20) << "Buble single" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(a_fst, SIZE))? "Ok" : "None") << endl;
	
	// Buble sort OMP
	tstart1 = omp_get_wtime();
	Sortirovki::BubleSort_parallel(b_fst, SIZE);
	tend1 = omp_get_wtime();
	cout << setw(20) << "Buble OMP" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(b_fst, SIZE)) ? "Ok" : "None") << endl;



	//перезаполнение массивов для новых тестов
	for (int i = 0; i < SIZE; i++)
	{

		a_fst[i] = rand() * 0.001;
		b_fst[i] = a_fst[i];

	}

	//отдедение строчки
	cout << endl;

	// Fast sort
	tstart1 = omp_get_wtime();
	Sortirovki::FastSort(a_fst, 0, SIZE - 1);
	tend1 = omp_get_wtime();
	cout << setw(20) << "Fast single" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(a_fst, SIZE)) ? "Ok" : "None") << endl;

	// Fast sort OMP
	tstart1 = omp_get_wtime();
	Sortirovki::FastSort_parallel(b_fst, 0, SIZE - 1);
	tend1 = omp_get_wtime();
	cout << setw(20) << "Fast OMP" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(b_fst, SIZE)) ? "Ok" : "None") << endl;



	//перезаполнение массивов для новых тестов
	for (int i = 0; i < SIZE; i++)
	{

		a_fst[i] = rand() * 0.001;
		b_fst[i] = a_fst[i];

	}

	//отдедение строчки
	cout << endl;

	// Time sort
	tstart1 = omp_get_wtime();
	Sortirovki::TimeSort(a_fst, SIZE);
	tend1 = omp_get_wtime();
	cout << setw(20) << "Time single" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(a_fst, SIZE)) ? "Ok" : "None") << endl;


	// Time sort OMP
	tstart1 = omp_get_wtime();
	Sortirovki::TimeSort_parallel(b_fst, SIZE);
	tend1 = omp_get_wtime();
	cout << setw(20) << "Time OMP" << setw(15) << tend1 - tstart1 << setw(15) << ((Sortirovki::CheckArray(b_fst, SIZE)) ? "Ok" : "None") << endl;

	//удаление массивов
	delete[] a_fst;
	delete[] b_fst;

	return 0;

}