#ifndef _KUSHNER_H
#define _KUSHNER_H

#include "classes.h"
#include <stdexcept>

class Kushner : public IMethod{
private:
  /// Параметр метода
  double r;
protected:
  double CalculateR(int p) override;
public:
  Kushner(TTask* Task, int count, double _r, double _eps);
  void FirstIteration() override;
  void DoIteration() override;
  void Solve() override;
  void InitIteration() override;
  void CalculateIterationPoint() override;
  bool CheckStopCondition() override;
  void FinalizeIteration() override;
};

#endif