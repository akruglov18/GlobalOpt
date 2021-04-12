#include "classes.h"
#include<fstream>

int IMethod::GetNumberOfTrials()
{
  return Trials.size();
}

int IMethod::GetBestTrialIteration()
{
  return BestTrial.IterationNumber;
}

void IMethod::PrintPoints(const std::string& fileName)
{
  std::ofstream out(fileName);
  for (auto& t : Trials) {
    out << t.x << " ";
  }
  out.close();
}

double IMethod::GetAchievedAccuracy()
{
  return fabs(BestTrial.x - pTask->GetOptimumPoint());
}

std::vector<TTrial> IMethod::GetTrials() const
{
    return Trials;
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

bool IMethod::CheckOptimumPoint(const TTrial& trial)
{
  if (trial.x + eps >= pTask->GetOptimumPoint() && pTask->GetOptimumPoint() >= trial.x - eps)
    return true;
  return false;
}
