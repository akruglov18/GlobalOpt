#include "classes.h"
#include <stdexcept>

class AGP : public IMethod{
private:
  /// �������� � ������������ ���������������
  int t;
  /// ������ ��������� �������
  double m;
  /// �������� ������
  double r;
  /// ������� ���������
  TTrial CurTrial;
  /// ������ ���������
  TTrial BestTrial;
  /// ��� �� ������� ������� �� ������� ��������
  bool recalc = false;
  /// ���������� ��������
  int IterationCount;
  /// 
  double eps;
protected:
  double CalculateR(int p) override;
  bool UpdateOptimumEstimation(const TTrial& trial) override;
public:
  AGP(TTask* Task, int count, int _r, double _eps);
  void FirstIteration() override;
  void DoIteration() override;
  void Solve() override;
  void InitIteration() override;
  void CalculateIterationPoint() override;
  void CalculateFunction() override;
  bool CheckStopCondition() override;
  void RenewSearchData() override;
  bool EstimateOptimum() override;
  void FinalizeIteration() override;
  int GetIterationCount() override;
  TTrial GetOptimEstimation() override;
  TTrial GetCurTrial() override;
  int GetNumberOfTrials() override;
  int GetBestTrialIteration() override;
};