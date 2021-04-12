#ifndef _CLASSES_H
#define _CLASSES_H

#include <vector>
#include <string>
/**
������� �����-���������, �� �������� ����������� ������, ����������� ������ �����������.
� ������ #TIProblem ������� ��������� �������, ������� ������ ���� ����������� � ����������� ������� � ��������.
*/
class IProblem
{
public:

	/// ��� ������, ������������, ���� �������� ��������� �������
	static const int OK = 0;
	/** ��� ������, ������������ �������� #GetOptimumValue � #GetOptimumPoint,
	���� ��������������� ��������� ������ �� ����������,
	*/
	static const int UNDEFINED = -1;
	/// ��� ������, ������������, ���� �������� �� ���������
	static const int ERROR = -2;

	///������������� ������
	virtual int Initialize() = 0;

	/// ����� ���������� ������� ������� ������
	virtual void GetBounds(double& lower, double& upper) = 0;
	/** ����� ���������� �������� ������� � ����� ����������� ��������
	\param[out] value ����������� ��������
	\return ��� ������ (#OK ��� #UNDEFINED)
	*/
	virtual int GetOptimumValue(double& value) const
	{
		return IProblem::UNDEFINED;
	}
	/** ����� ���������� ���������� ����� ����������� �������� ������� �������
	\param[out] y �����, � ������� ����������� ����������� ��������
	\return ��� ������ (#OK ��� #UNDEFINED)
	*/
	virtual int GetOptimumPoint(double& y) const = 0;

	/** �����, ����������� ������� ������
	\param[in] y �����, � ������� ���������� ��������� ��������
	\return �������� �������
	*/
	virtual double CalculateFunction(const double y) const = 0;

	///����������
	virtual ~IProblem() {}
};

// ------------------------------------------------------------------------------------------------
struct TTrial
{
	/// ����� �� �������
	double  x;
	/// �������� ������� ������
	double FuncValue;
	/// ����� ��������
	int IterationNumber;

};

// ------------------------------------------------------------------------------------------------
class TTask
{
protected:
	/// ����� ������� ������� ������
	double     A;
	/// ������ ������� ������� ������
	double     B;
	/// ��������� �� ���� ������ �����������
	IProblem* pProblem;
	/// ����������� �������� ������� ������� (����������, ���� �������� �� ������)
	double     OptimumValue;
	/// ���������� ����������� �������� ������� ������� (����������, ���� ��������)
	double     OptimumPoint;
	/// true, ���� � ������ �������� ����������� �������� ��������
	bool       IsOptimumValueDefined;
	/// true, ���� � ������ �������� ����� ����������� ��������
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

	/// ���������� ����� ������� ������� ������
	const double GetA() const { return A; }
	/// ���������� ������ ������� ������� ������
	const double GetB() const { return B; }
	/// ���������� ������� ��������� �������� ����������� ��������
	double GetOptimumValue() const { return OptimumValue; }
	/// ���������� ������� ��������� ���������� ����� ����������� ��������
	const double GetOptimumPoint()
	{
		pProblem->GetOptimumPoint(OptimumPoint);
		return OptimumPoint;
	}
	/// ���������� �������� �� ��� ������ �������� ����������� ��������
	bool GetIsOptimumValueDefined() const { return IsOptimumValueDefined; }
	/// ���������� �������� �� ��� ������ ���������� ����������� ��������
	bool GetIsOptimumPointDefined() const { return IsOptimumPointDefined; }
	/// ���������� ������� ������
	IProblem* getProblem() { return pProblem; }
	/// ��������� �������� �������
	virtual double CalculateFunction(const double y)
	{
		return pProblem->CalculateFunction(y);
	}
};


/**
��������� ������������������� ��������� ���������� �����������.
� ������ #IMethod ��������� �������� �������, ������������ ������ ������������������� ���������.
*/
class IMethod
{
protected:
	///��������� �� �������� ������
	TTask* pTask;
	///
	std::vector<TTrial> Trials;
	/// ������� ���������
	TTrial CurTrial;
	/// ������ ���������
	TTrial BestTrial;
	/// ��� �� ������� ������� �� ������� ��������
	bool recalc = false;
	/// ���������� ��������
	int IterationCount;
	/// �������� � ������������ ���������������
	int t;
	///
	double eps;
	/** ���������� �������������� ���������
	\param[in] p ��������� �� ��������, �������������� �������� ���� ���������
	\return �������������� ���������
	*/
	virtual double CalculateR(int p) = 0;

	/** ���������� ������� ������ ��������
	���� ���������� ����� ����� ������� ������ ��������, �� ��� ������ ����������� � ����������� ���� #recalc.
	\param[in] trial �����, ������� ���������� �������� � ������� ���������
	\return true, ���� ������� �������, ����� false
	*/
	bool UpdateOptimumEstimation(const TTrial& trial);

	/// ��������� ������ �� ����� � ����������� ����������� ��������
	virtual bool CheckOptimumPoint(const TTrial& trial);

public:

	/** ������� ��������� ������ �������� ������
	*/
	virtual void FirstIteration() = 0;
	/** ������� ��������� ������� �������� ������
	*/
	virtual void DoIteration() = 0;

	/** ������� ��������� ������� ������� ������
	*/
	virtual void Solve() = 0;

	/** ������� ���������� � ������ ���������� ��������
	*/
	virtual void InitIteration() = 0;
	/** ���������� ����� ��������� ��������
	��������� ������������ � CurTrial
	*/
	virtual void CalculateIterationPoint() = 0;
	/** �������� ���������� �������� ��������� ������
	����� ���������� ������ � ��������� �������:
	- ����� ��������� ��������� ����������� ���������� ��������
	- ���� �������� ���� ������ � �������� �������� \f$ x_t - x_{t-1} < \epsilon \f$
	- ���� �������� ����� ����� � �������� �������� \f$ \| y^k - y^\ast \| < \epsilon \f$
	\return ������, ���� �������� ��������� ��������; ���� - � ��������� ������.
	*/
	virtual bool CheckStopCondition() = 0;

	/** ���������� ������� ������
	���������� ��������� � ����� �� CurTrial, ���������	������������ ���� ��
	*/
	void CalculateFunction();
	/** ���������� ��������� ����������
	*/
	void RenewSearchData();
	/** ������� ������� �������� ��������
	\return ������, ���� ������� ���������; ���� - � ��������� ������
	*/
	bool EstimateOptimum();
	/** ������� ���������� � ����� ���������� ��������
	*/
	virtual void FinalizeIteration() = 0;
	/** �������� ����� ���������
	\return ����� ���������
	*/
	int GetIterationCount();
	/** �������� ������� ������ ��������
	\return ���������, ��������������� �������� ��������
	*/
	TTrial GetOptimEstimation();
	/** �������� ������� ���������
	\return ������� ���������
	*/
	TTrial GetCurTrial();
	/**���� ����������
	������� ���������� ����� ����� ���������, ����������� ��� ������� ������� ������
	\return ����� ����� ���������
	*/
	int GetNumberOfTrials();
	///�������� ����� �������� � ��������� ��������
	int GetBestTrialIteration();
	/**������� ���������� ����� ��������� � ����
	\param[in] fileName ��� �����, � ������� ����� �������� �����
	*/
	virtual void PrintPoints(const std::string& fileName);

	/// ���������� ����������� ��������
	virtual double GetAchievedAccuracy();

	/// ���������� ������ ���������
	std::vector<TTrial> GetTrials() const;
};

#endif