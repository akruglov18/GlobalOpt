#include "classes.h"
#include <stdexcept>

class AGP : public IMethod{
private:
  /// Оценка константы Липшица
  double m;
  /// Параметр метода
  double r;
  /// 
  double eps;
protected:
  double CalculateR(int p) override;
public:
  AGP(TTask* Task, int count, int _r, double _eps);
  void FirstIteration() override;
  void DoIteration() override;
  void Solve() override;
  void InitIteration() override;
  void CalculateIterationPoint() override;
  bool CheckStopCondition() override;
  void FinalizeIteration() override;
};