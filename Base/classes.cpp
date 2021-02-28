#include "classes.h"

int IMethod::GetNumberOfTrials()
{
  return Trials.size();
}

int IMethod::GetBestTrialIteration()
{
  return BestTrial.IterationNumber;
}

void IMethod::CalculateFunction()
{
  CurTrial.FuncValue = pTask->CalculateFunction(CurTrial.x);
}

void IMethod::RenewSearchData()
{
  Trials.insert(Trials.begin() + t, CurTrial);
}

bool IMethod::EstimateOptimum()
{
  return recalc;
}

int IMethod::GetIterationCount()
{
  return IterationCount;
}

TTrial IMethod::GetOptimEstimation()
{
  return BestTrial;
}

TTrial IMethod::GetCurTrial()
{
  return CurTrial;
}

bool IMethod::UpdateOptimumEstimation(const TTrial& trial)
{
  if (trial.FuncValue < BestTrial.FuncValue) {
    BestTrial = trial;
    recalc = true;
    return true;
  }
  return false;
}
