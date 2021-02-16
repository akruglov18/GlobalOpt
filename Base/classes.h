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
	virtual bool UpdateOptimumEstimation(const TTrial& trial) = 0;

	/// ��������� ������ �� ����� � ����������� ����������� ��������
	virtual bool CheckOptimumPoint(const TTrial& trial) = 0;

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
	virtual void CalculateFunction() = 0;
	/** ���������� ��������� ����������
	*/
	virtual void RenewSearchData() = 0;
	/** ������� ������� �������� ��������
	\return ������, ���� ������� ���������; ���� - � ��������� ������
	*/
	virtual bool EstimateOptimum() = 0;
	/** ������� ���������� � ����� ���������� ��������
	*/
	virtual void FinalizeIteration() = 0;
	/** �������� ����� ���������
	\return ����� ���������
	*/
	virtual int GetIterationCount() = 0;
	/** �������� ������� ������ ��������
	\return ���������, ��������������� �������� ��������
	*/
	virtual TTrial GetOptimEstimation() = 0;
	/** �������� ������� ���������
	\return ������� ���������
	*/
	virtual TTrial GetCurTrial() = 0;
	/** ���������� ������� ������� ������
	*/
	virtual void SetBounds() = 0;
	/**���� ����������
	������� ���������� ����� ����� ���������, ����������� ��� ������� ������� ������
	\return ����� ����� ���������
	*/
	virtual int GetNumberOfTrials() = 0;
	///�������� ����� �������� � ��������� ��������
	virtual int GetBestTrialIteration() = 0;
	/**������� ���������� ����� ��������� � ����
	\param[in] fileName ��� �����, � ������� ����� �������� �����
	*/
	virtual void PrintPoints(const std::string& fileName) = 0;

	/// ���������� ����������� ��������
	virtual double GetAchievedAccuracy() = 0;
};
