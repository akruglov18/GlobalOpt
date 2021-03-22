#include "classes.h"
#include <iostream>
#include "AGP.h"

//Функция y = (x-1)(x-1) при x \in [-1,2]
class TX2Problem : public IProblem
{

	///Инициализация задачи
	virtual int Initialize()
	{
		return IProblem::OK;
	}

	/// Метод возвращает границы области поиска
	virtual void GetBounds(double& lower, double& upper)
	{
		lower = -1;
		upper = 2;
	}
	/** Метод возвращает значение функции в точке глобального минимума
	\param[out] value оптимальное значение
	\return Код ошибки (#OK или #UNDEFINED)
	*/
	virtual int GetOptimumValue(double& value) const
	{
		value = 0;
		return IProblem::OK;
	}
	/** Метод возвращает координаты точки глобального минимума целевой функции
	\param[out] x точка, в которой достигается оптимальное значение
	\return Код ошибки (#OK или #UNDEFINED)
	*/
	virtual int GetOptimumPoint(double& x) const
	{
		x = 1;
		return IProblem::OK;

	}

	/** Метод, вычисляющий функцию задачи
	\param[in] x Точка, в которой необходимо вычислить значение
	\return Значение функции
	*/
	virtual double CalculateFunction(const double x) const
	{
		return x * x;
	}

	///Деструктор
	~TX2Problem()
	{
	}

};

//Пример создания конкретной задачи
//int main(int argc, char* argv[])
//{
//	IMethod* meth = new AGP();
//	//Объявляем указатель на базовый класс и присваиваем ему указатель на производный класс
//	IProblem* problem = new TX2Problem();
//	//Инициализируем задачу оптимизации
//	TTask task(problem);
//	//И используем ее в методе
//
//	//Для примера - печать таблицы значений функции
//	int size = 20;
//	double x = task.GetA();
//	double h = (task.GetB() - task.GetA()) / size;
//
//	std::cout << " x        f(x)" << std::endl;
//	std::cout.precision(2);
//	std::cout << std::fixed;
//	for (int i = 0; i < size; i++)
//	{
//		x = task.GetA() + i * h;
//		std::cout << x << "     " << task.CalculateFunction(x) << std::endl;
//	}
//	std::cout << std::endl << std::endl;
//
//	std::cout << "min{f(x)} = " << task.GetOptimumValue() << std::endl;
//	std::cout << "arg min{f(x)} = " << task.GetOptimumPoint() << std::endl << std::endl;
//
//
//	delete problem;
//
//	return 0;
//}

int main() {
	IProblem* problem = new TX2Problem();
	TTask task(problem);
	IMethod* method = new AGP(&task, 300, 2.0, 0.001);
	method->Solve();
	std::cout << "z: " << method->GetOptimEstimation().FuncValue << " x:" << method->GetOptimEstimation().x << "\n";
	std::cout << "count: " << method->GetNumberOfTrials();
}