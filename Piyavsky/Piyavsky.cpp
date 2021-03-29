#include "Piyavsky.h"

double Piyavsky::CalculateR(int p)
{
  if (p <= 0 || p >= Trials.size()) {
    throw new std::out_of_range("vector Trials out of range");
  }
  return 0.5 * m * (Trials[p].x - Trials[p - 1].x) - 0.5 * (Trials[p].FuncValue + Trials[p - 1].FuncValue);
}

Piyavsky::Piyavsky(TTask* Task, int count, double _r, double _eps)
{
  pTask = Task;
  IterationCount = count;
  r = _r;
  eps = _eps;
  Trials.reserve(IterationCount);
}

void Piyavsky::FirstIteration()
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

void Piyavsky::DoIteration()
{
  CalculateIterationPoint();
  CalculateFunction();
  RenewSearchData();

  double M = -1e8;
  for (int i = 1; i < Trials.size(); i++) {
    double M1 = abs((Trials[i].FuncValue - Trials[i - 1].FuncValue) / (Trials[i].x - Trials[i- 1].x));
    if (M1 > M) M = M1;
  }
  if (M > 0) m = r * M;
  else m = 1;

  double maxR = -1e8;
  for (int i = 1; i < Trials.size(); i++) {
    double curR = CalculateR(i);
    if (curR > maxR) {
      maxR = curR;
      t = i;
    }
  }
}

void Piyavsky::Solve()
{
  FirstIteration();
  while (!CheckStopCondition()) {
    InitIteration();
    DoIteration();
    FinalizeIteration();
  }
}

void Piyavsky::InitIteration()
{
  recalc = false;
  CurTrial.IterationNumber++;
}

void Piyavsky::CalculateIterationPoint()
{
  if (t <= 0 || t>Trials.size()) {
    throw new std::out_of_range("vector Trials out of range");
  }
  CurTrial.x = 0.5 * (Trials[t - 1].x + Trials[t].x) - 0.5 * (Trials[t].FuncValue - Trials[t - 1].FuncValue) / m;
}

bool Piyavsky::CheckStopCondition()
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

void Piyavsky::FinalizeIteration()
{
  UpdateOptimumEstimation(CurTrial);
}
