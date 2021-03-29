#include "Kushner.h"

double Kushner::CalculateR(int p)
{
  double zmin = BestTrial.FuncValue;
  double up = (-4) * (zmin - r - Trials[p - 1].FuncValue) * (zmin - r - Trials[p].FuncValue);
  double down = Trials[p].x - Trials[p - 1].x;
  return up / down;
}

Kushner::Kushner(TTask* Task, int count, double _r, double _eps)
{
  pTask = Task;
  IterationCount = count;
  r = _r;
  eps = _eps;
  Trials.reserve(IterationCount);
}

void Kushner::FirstIteration()
{
  double A = pTask->GetA();
  double FuncA = pTask->CalculateFunction(A);
  double B = pTask->GetB();
  double FuncB = pTask->CalculateFunction(B);

  Trials.push_back({ A, FuncA, 1 });
  Trials.push_back({ B, FuncB, 2 });

  if (FuncA < FuncB)
    BestTrial = { A, FuncA, 1 };
  else
    BestTrial = { B, FuncB, 2 };

  CurTrial = { B, FuncB, 2 };
  t = 1;
}

void Kushner::DoIteration()
{
  CalculateIterationPoint();
  CalculateFunction();
  RenewSearchData();

  double maxR = -1e8;
  for (int i = 1; i < Trials.size(); i++) {
    double curR = CalculateR(i);
    if (curR > maxR) {
      maxR = curR;
      t = i;
    }
  }
}

void Kushner::Solve()
{
  FirstIteration();
  while (!CheckStopCondition()) {
    InitIteration();
    DoIteration();
    FinalizeIteration();
  }
}

void Kushner::InitIteration()
{
  recalc = false;
  CurTrial.IterationNumber++;
}

void Kushner::CalculateIterationPoint()
{
  double zmin = BestTrial.FuncValue;
  double up = (Trials[t].x - Trials[t - 1].x) * (zmin - r - Trials[t - 1].FuncValue);
  double down = 2 * (zmin - r) - Trials[t].FuncValue - Trials[t - 1].FuncValue;
  CurTrial.x = Trials[t - 1].x + up / down;
}

bool Kushner::CheckStopCondition()
{
  if (t <= 0 || t > Trials.size())
    throw new std::out_of_range("vector Trials out of range");
  double e = eps * (pTask->GetB() - pTask->GetA());
  if (Trials[t].x - Trials[t - 1].x <= e)
    return true;

  if (Trials.size() >= IterationCount)
    return true;

  return false;
}

void Kushner::FinalizeIteration()
{
  UpdateOptimumEstimation(CurTrial);
}
