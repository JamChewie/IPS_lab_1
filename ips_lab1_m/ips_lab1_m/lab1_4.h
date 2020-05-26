#ifndef LAB1_4_H
#define LAB1_4_H

#include <iostream>
#include <math.h>
#include <ctime>
#include <vector>
#include <tbb/tbb.h>
#include <mutex>   
namespace LAB1_4 {
	std::mutex mtx;
	// Функция вычисления значения функции в точке
	template <class T>
	double f(T x)
	{
		return 4 / pow(1 + pow(x, 2), 2);
	}

	//Функция вычисления значения интеграла
	template <class T>
	double integral(T left_border, T right_border, T h)
	{
		int n = (right_border - left_border) / h;
		cilk::reducer_opadd<double> result(0.0);
		std::cout << n << std::endl;
		tbb::parallel_for(int(0), n, [&](auto i)	
		{	
			mtx.lock();
			result += f<double>(left_border + i * h + h / 2);
			mtx.unlock();
		});
		return result.get_value() * h;
	}

	int run()
	{
		double left_border = -1;
		double right_border = 1;
		//std::vector <double> Borders = { left_border,-0.5,0,0.5,right_border };		// Создаем вектор пределов интегрирования
		double h = 0.01;
		std::cout.setf(std::ios::scientific); 										// Настройка вывода
		std::cout << "=============================================" << std::endl << std::endl;
		for (h = 0.01; h > 0.000001; h = h / 10) 									// Запуск цикла для вычисления интеграла с разбиеним 100,1000,10000,100000,1000000
		{
			double result = 0.0;
			auto start = clock() / 1000.0;
			result = integral <double>(left_border, right_border, h);									// Вычисление интеграла			
			auto run_time = clock() / 1000.0;
			std::cout << "result = " << result << "  runtime = " << run_time - start << " h= " << h << std::endl;
			std::cout << std::endl << "=============================================" << std::endl << std::endl;
		}
		return 0;
	}
}
#endif // !LAB1_4_H