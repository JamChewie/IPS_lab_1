#ifndef LAB1_2_H
#define LAB1_2_H

#include <iostream>
#include <math.h>
#include <thread>
#include <ctime>         
#include <vector>
namespace LAB1_2 {
	// Функция вычисления значения функции
	template <class T>
	double f(T x)
	{
		return 4 / pow((1 + x * x), 2);
	}

	// Функция вычисления значения интеграла без добавления параллелизма
	template <class T>
	double integral(T left_border, T right_border, T h)
	{
		double result = 0;
		for (auto i = left_border; i < right_border; i += h)
		{
			result += f<double>(i + h / 2);
		}
		return result * h;
	}

	// Функция вычисления значения интеграла с добавления автоматического параллелизатора Qpar
	template <class T>
	double integral_qpar(T left_border, T right_border, T h)
	{
		double result = 0;
#pragma loop(hint_parallel(2))
		for (auto i = left_border; i < right_border; i += h)
		{
			result += f<double>(i + h / 2);
		}
		return result * h;
	}

	// Функция вычисления значения интеграла с отключенной векторизацией
	template <class T>
	double integral_no_vector(T left_border, T right_border, T h)
	{
		double result = 0;
#pragma loop(no_vector)
		for (auto i = left_border; i < right_border; i += h)
		{
			result += f<double>(i + h / 2);
		}
		return result * h;
	}

	int run()
	{
		std::vector <double> Thread_borders = { -0.75,-0.5, -0.25,0,0.25,0.5,0.75 }; // Создаем вектор из пределов интегрирования, для разбиения
		long double result = 0;														// вычисления интеграла при помощи потоков
		long double left_border = -1;
		long double right_border = 1;
		long double h = 0.1;
		std::cout.setf(std::ios::scientific);
		for (h = 0.01; h > 0.000001; h = h / 10)
		{
			std::cout << "Threads " << std::endl;
			auto start = clock() / 1000.0;
			std::thread t1([&]() {result += integral<double>(left_border, Thread_borders[0], h); });	// Создаем 8 потоков для параллельного вычисления значения интеграла
			std::thread t2([&]() {result += integral<double>(Thread_borders[0], Thread_borders[1], h); });
			std::thread t3([&]() {result += integral<double>(Thread_borders[1], Thread_borders[2], h); });
			std::thread t4([&]() {result += integral<double>(Thread_borders[2], Thread_borders[3], h); });
			std::thread t5([&]() {result += integral<double>(Thread_borders[3], Thread_borders[4], h); });
			std::thread t6([&]() {result += integral<double>(Thread_borders[4], Thread_borders[5], h); });
			std::thread t7([&]() {result += integral<double>(Thread_borders[5], Thread_borders[6], h); });
			std::thread t8([&]() {result += integral<double>(Thread_borders[6], right_border, h); });
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			auto end_time_1 = clock() / 1000.0;
			std::cout << "result = " << result << "  runtime = " << end_time_1 - start << " h= " << h << std::endl;
			std::cout << "Qpar  " << std::endl;
			result = 0;
			result += integral_qpar<double>(left_border, right_border, h);
			auto end_time_2 = clock() / 1000.0;
			std::cout << "result = " << result << "  runtime = " << end_time_2 - end_time_1 << " h= " << h << std::endl;
			std::cout << "No vector  " << std::endl;
			result = 0;
			result += integral_no_vector<double>(left_border, right_border, h);
			std::cout << "result = " << result << "  runtime = " << clock() / 1000.0 - end_time_2 << " h= " << h << std::endl;
			std::cout << std::endl << "==================================" << std::endl;
			result = 0;
			left_border = -1;
		}
		system("pause");
		return 0;
	}
}
#endif