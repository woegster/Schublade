#pragma once
#include <pdh.h>
#include <vector>

namespace toni
{
  class CpuUsageSampler
  {
  public:
    CpuUsageSampler();
    ~CpuUsageSampler();

    std::vector<double> SampleCPUs();
  private:
    PDH_HQUERY m_cpuQuery;
    std::vector<PDH_HCOUNTER> m_cpuCounters;
  };
}