#include <iostream>
#include <math.h>
#include <ctime>
#include <cilk/cilk.h>
#include <vector>
#include <cilk/reducer_opadd.h>

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
	double result = 0;
	for (auto i = left_border ; i < right_border; i += h){
		result += f<double>(i + h / 2);		
	}
	return result * h;
}

int main() 
{	
	double left_border = -1;
	double right_border = 1;
	std::vector <double> Borders = {left_border,-0.5,0,0.5,right_border};	// Создаем вектор пределов интегрирования
	double h = 0.01;
	std::cout.setf(std::ios::scientific); 									// Настройка вывода
	std::cout << "=============================================" << std::endl << std::endl;
	for (h = 0.01; h > 0.000001; h = h / 10){ 								// Запуск цикла для вычисления интеграла с разбиеним 100,1000,10000,100000,1000000
		cilk::reducer_opadd<double> result(0.0);							// Создаем переменную типа reducer_opadd для избежания гонок данных
		auto start = clock() / 1000.0;				
		cilk_for (int i = 0; i < 4; i++) 									// Используем цикл с встроенным распараллеливанием cilk_for
		{											
			result += integral(Borders[i], Borders[i+1], h);		
		}
		auto run_time = clock() / 1000.0;
		std::cout << "result = " << result.get_value() << "  runtime = " << run_time - start << " h= " << h << std::endl;
		std::cout << std::endl << "=============================================" << std::endl << std::endl;	
	}			
	return 0;
}