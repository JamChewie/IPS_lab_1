#ifndef LAB1_3_H
#define LAB1_3_H

#include <iostream>
#include <math.h>
#include <ctime>
#include <cilk/cilk.h>
#include <vector>
#include <cilk/reducer_opadd.h>
namespace LAB1_3 {
	// Функция вычисления значения функции
	template <class T>
	double f(T x)
	{
		return 4 / pow(1 + pow(x, 2), 2);
	}

	// Функция вычисления значения интеграла
	template <class T>
	double integral(T left_border, T right_border, T h)
	{
		// Создаем переменную типа reducer_opadd для избежания гонок данных
		cilk::reducer_opadd<double> result(0.0);							
		int length = (right_border - left_border)/h;
		cilk_for (auto i = 0; i < length; i ++) {
			result += f<double>(left_border+i*h + h / 2);
		}
		return result.get_value() * h;
	}

	int run()
	{
		double left_border = -1;
		double right_border = 1;
		double h = 0.01;
		std::cout.setf(std::ios::scientific); 									// Настройка вывода
		std::cout << "=============================================" << std::endl << std::endl;
		for (h = 0.01; h > 0.000001; h = h / 10) { 								// Запуск цикла для вычисления интеграла с разбиеним 100,1000,10000,100000,1000000
			auto start = clock() / 1000.0;
			
			auto result = integral(left_border,right_border, h);
			
			auto run_time = clock() / 1000.0;
			std::cout << "result = " << result << "  runtime = " << run_time - start << " h= " << h << std::endl;
			std::cout << std::endl << "=============================================" << std::endl << std::endl;
		}
		return 0;
	}
}
#endif