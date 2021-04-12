#ifndef _CLASSES_H
#define _CLASSES_H

#include <vector>
#include <string>
/**
Базовый класс-интерфейс, от которого наследуются классы, описывающие задачи оптимизации.
В классе #TIProblem описаны прототипы методов, которые должны быть реализованы в подключамых модулях с задачами.
*/
class IProblem
{
public:

	/// Код ошибки, возвращаемый, если операция завершена успешно
	static const int OK = 0;
	/** Код ошибки, возвращаемый методами #GetOptimumValue и #GetOptimumPoint,
	если соответствующие параметры задачи не определены,
	*/
	static const int UNDEFINED = -1;
	/// Код ошибки, возвращаемый, если операция не выполнена
	static const int ERROR = -2;

	///Инициализация задачи
	virtual int Initialize() = 0;

	/// Метод возвращает границы области поиска
	virtual void GetBounds(double& lower, double& upper) = 0;
	/** Метод возвращает значение функции в точке глобального минимума
	\param[out] value оптимальное значение
	\return Код ошибки (#OK или #UNDEFINED)
	*/
	virtual int GetOptimumValue(double& value) const
	{
		return IProblem::UNDEFINED;
	}
	/** Метод возвращает координаты точки глобального минимума целевой функции
	\param[out] y точка, в которой достигается оптимальное значение
	\return Код ошибки (#OK или #UNDEFINED)
	*/
	virtual int GetOptimumPoint(double& y) const = 0;

	/** Метод, вычисляющий функцию задачи
	\param[in] y Точка, в которой необходимо вычислить значение
	\return Значение функции
	*/
	virtual double CalculateFunction(const double y) const = 0;

	///Деструктор
	virtual ~IProblem() {}
};

// ------------------------------------------------------------------------------------------------
struct TTrial
{
	/// точка на отрезке
	double  x;
	/// значения функции задачи
	double FuncValue;
	/// Номер итерации
	int IterationNumber;

};

// ------------------------------------------------------------------------------------------------
class TTask
{
protected:
	/// левая граница области поиска
	double     A;
	/// правая граница области поиска
	double     B;
	/// указатель на саму задачу оптимизации
	IProblem* pProblem;
	/// оптимальное значение целевой функции (определено, если известно из задачи)
	double     OptimumValue;
	/// координата глобального минимума целевой функции (определено, если известно)
	double     OptimumPoint;
	/// true, если в задаче известно оптимальное значение критерия
	bool       IsOptimumValueDefined;
	/// true, если в задаче известна точка глобального минимума
	bool       IsOptimumPointDefined;

public:
	TTask(IProblem* _problem)
	{
		pProblem = _problem;
		pProblem->GetBounds(A, B);
		if (pProblem->GetOptimumPoint(OptimumPoint) == IProblem::OK)
		{
			IsOptimumPointDefined = true;
		}
		if (pProblem->GetOptimumValue(OptimumValue) == IProblem::OK)
		{
			IsOptimumValueDefined = true;
		}

	}
	virtual ~TTask()
	{

	}

	/// Возвращает левую границу области поиска
	const double GetA() const { return A; }
	/// Возвращает правую границу области поиска
	const double GetB() const { return B; }
	/// Возвращает априори известное значение глобального минимума
	double GetOptimumValue() const { return OptimumValue; }
	/// Возвращает априори известные координаты точки глобального минимума
	const double GetOptimumPoint()
	{
		pProblem->GetOptimumPoint(OptimumPoint);
		return OptimumPoint;
	}
	/// Возвращает известно ли для задачи значение глобального минимума
	bool GetIsOptimumValueDefined() const { return IsOptimumValueDefined; }
	/// Возвращает известны ли для задачи координаты глобального минимума
	bool GetIsOptimumPointDefined() const { return IsOptimumPointDefined; }
	/// Возвращает текущую задачу
	IProblem* getProblem() { return pProblem; }
	/// Вычисляет значение функции
	virtual double CalculateFunction(const double y)
	{
		return pProblem->CalculateFunction(y);
	}
};


/**
Интерфейс характеристического алгоритма глобальной оптимизации.
В классе #IMethod объявлены основные функции, определяющие работу характеристического алгоритма.
*/
class IMethod
{
protected:
	///Указатель на решаемую задачу
	TTask* pTask;
	///
	std::vector<TTrial> Trials;
	/// Текущее испытание
	TTrial CurTrial;
	/// Лучшее испытание
	TTrial BestTrial;
	/// Был ли обновлён оптимум на текущей итерации
	bool recalc = false;
	/// Количество итераций
	int IterationCount;
	/// Интервал с максимальной характеристикой
	int t;
	///
	double eps;
	/** Вычисление характеристики интервала
	\param[in] p указатель на интервал, характеристику которого надо вычислить
	\return Характеристика интервала
	*/
	virtual double CalculateR(int p) = 0;

	/** Обновление текущей оценки оптимума
	Если переданная точка лучше текущей оценки оптимума, то эта оценка обновляется и поднимается флаг #recalc.
	\param[in] trial точка, которую необходимо сравнить с текущим оптимумом
	\return true, если оптимум обновлён, иначе false
	*/
	bool UpdateOptimumEstimation(const TTrial& trial);

	/// Проверяет попала ли точка в окрестность глобального манимума
	virtual bool CheckOptimumPoint(const TTrial& trial);

public:

	/** Функция выполняет первую итерацию метода
	*/
	virtual void FirstIteration() = 0;
	/** Функция выполняет текущую итерацию метода
	*/
	virtual void DoIteration() = 0;

	/** Функция выполняет решение текущей задачи
	*/
	virtual void Solve() = 0;

	/** Функция вызывается в начале проведения итерации
	*/
	virtual void InitIteration() = 0;
	/** Вычисления точки очередной итерации
	Результат записывается в CurTrial
	*/
	virtual void CalculateIterationPoint() = 0;
	/** Проверка выполнения критерия остановки метода
	Метод прекращает работу в следующих случаях:
	- число испытаний превысило максимально допустимое значение
	- если решается одна задача и выполнен критерий \f$ x_t - x_{t-1} < \epsilon \f$
	- если решается серия задач и выполнен критерий \f$ \| y^k - y^\ast \| < \epsilon \f$
	\return истина, если критерий остановки выполнен; ложь - в противном случае.
	*/
	virtual bool CheckStopCondition() = 0;

	/** Вычисление функций задачи
	Проводится испытание в точке из CurTrial, результат	записываются туда же
	*/
	void CalculateFunction();
	/** Обновление поисковой информации
	*/
	void RenewSearchData();
	/** Оценить текущее значение оптимума
	\return истина, если оптимум изменился; ложь - в противном случае
	*/
	bool EstimateOptimum();
	/** Функция вызывается в конце проведения итерации
	*/
	virtual void FinalizeIteration() = 0;
	/** Получить число испытаний
	\return число испытаний
	*/
	int GetIterationCount();
	/** Получить текущую оценку оптимума
	\return испытание, соответствующее текущему оптимуму
	*/
	TTrial GetOptimEstimation();
	/** Получить текущее испытание
	\return текущее испытание
	*/
	TTrial GetCurTrial();
	/**Сбор статистики
	Функция возвращает общее число испытаний, выполненных при решении текущей задачи
	\return общее число испытаний
	*/
	int GetNumberOfTrials();
	///Получить номер итерации с наилучшим решением
	int GetBestTrialIteration();
	/**Функция записывает точки испытаний в файл
	\param[in] fileName имя файла, в который будут записаны точки
	*/
	virtual void PrintPoints(const std::string& fileName);

	/// Возвращает достигнутую точность
	virtual double GetAchievedAccuracy();

	/// Возвращает вектор испытаний
	std::vector<TTrial> GetTrials() const;
};

#endif