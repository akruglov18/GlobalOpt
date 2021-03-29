#ifndef _Piyavsky_H
#define _AGP_H

#include "classes.h"
#include <stdexcept>

class Piyavsky : public IMethod{
private:
  /// ������ ��������� �������
  double m;
  /// �������� ������
  double r;
protected:
  double CalculateR(int p) override;
public:
  Piyavsky(TTask* Task, int count, double _r, double _eps);
  void FirstIteration() override;
  void DoIteration() override;
  void Solve() override;
  void InitIteration() override;
  void CalculateIterationPoint() override;
  bool CheckStopCondition() override;
  void FinalizeIteration() override;
};

#endif