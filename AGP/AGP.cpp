#include "AGP.h"

double AGP::CalculateR(int p)
{
  if (p <= 0 || p >= Trials.size()) {
    throw new std::out_of_range("vector Trials out of range");
  }
  double zd = Trials[p].FuncValue - Trials[p - 1].FuncValue;
  double xd = Trials[p].x - Trials[p - 1].x;
  return (m * xd) + (zd * zd) / (m * xd) - 2 * (Trials[p].FuncValue + Trials[p - 1].FuncValue);
}

AGP::AGP(TTask* Task, int count, int _r, double _eps)
{
  pTask = Task;
  IterationCount = count;
  r = _r;
  eps = _eps;
  Trials.reserve(IterationCount);
}

void AGP::FirstIteration()
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

  double M = abs((Trials[1].FuncValue - Trials[0].FuncValue) / (Trials[1].x - Trials[0].x));
  if (M > 0)
    m = r * M;
  else
    m = 1;

  CurTrial = { B, FuncB, 2 };
  t = 1;
}

void AGP::DoIteration()
{
  CalculateIterationPoint();
  CalculateFunction();
  RenewSearchData();

  double M = -DBL_MAX;
  for (int i = 1; i < Trials.size(); i++) {
    double M1 = abs((Trials[i].FuncValue - Trials[i - 1].FuncValue) / (Trials[i].x - Trials[i].x));
    if (M1 > M) M = M1;
  }
  if (M > 0) m = r * M;
  else m = 1;

  double maxR = -DBL_MAX;
  for (int i = 1; i < Trials.size(); i++) {
    double curR = CalculateR(i);
    if (curR > maxR) {
      maxR = curR;
      t = i;
    }
  }
}

void AGP::Solve()
{
  FirstIteration();
  while (!CheckStopCondition()) {
    InitIteration();
    DoIteration();
    FinalizeIteration();
  }
}

void AGP::InitIteration()
{
  recalc = false;
  CurTrial.IterationNumber++;
}

void AGP::CalculateIterationPoint()
{
  if (t <= 0 || t>Trials.size()) {
    throw new std::out_of_range("vector Trials out of range");
  }
  CurTrial.x = 0.5 * (Trials[t - 1].x + Trials[t].x) - 0.5 * (Trials[t].FuncValue - Trials[t - 1].FuncValue) / m;
}

bool AGP::CheckStopCondition()
{
  if (t <= 0 || t > Trials.size())
    throw new std::out_of_range("vector Trials out of range");

  if (Trials[t].x - Trials[t - 1].x < eps)
    return true;

  if (Trials.size() >= IterationCount)
    return true;

  return false;
}

void AGP::FinalizeIteration()
{
  UpdateOptimumEstimation(CurTrial);
}
