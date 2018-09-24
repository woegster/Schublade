#include "CpuUsageSampler.h"

namespace toni
{
  CpuUsageSampler::CpuUsageSampler()
  {
    PDH_STATUS s = PdhOpenQuery(NULL, NULL, &m_cpuQuery);

    if (s == ERROR_SUCCESS)
    {
      DWORD bfrSize = 0;
      const wchar_t* processorTimeWildcardString = L"\\Processor(*)\\% Processor Time";
      s = PdhExpandWildCardPath(nullptr, processorTimeWildcardString, nullptr, &bfrSize, 0);

      if (s == ERROR_SUCCESS)
      {
        wchar_t* bfr = new wchar_t[bfrSize];
        s = PdhExpandWildCardPath(nullptr, processorTimeWildcardString, bfr, &bfrSize, 0);

        for (DWORD i = 0; i < bfrSize;)
        {
          wchar_t* counterName = &bfr[i];
          int counterNameLength = lstrlenW(counterName);
          if (counterNameLength == 0)
          {
            break;
          }


          if (wcsstr(counterName, L"_Total") == nullptr)
          {
            PDH_HCOUNTER newCounter;
            s = PdhAddCounter(m_cpuQuery, counterName, NULL, &newCounter);
            if (s == ERROR_SUCCESS)
              m_cpuCounters.push_back(newCounter);
          }

          i += counterNameLength + 1; //+1 to point past the \0 and into the next name
        }

        delete[] bfr;
      }
    }
  }

  std::vector<double> CpuUsageSampler::SampleCPUs()
  {
    PDH_STATUS s = PdhCollectQueryData(m_cpuQuery);

    std::vector<double> values;
    values.reserve(m_cpuCounters.size());

    for (auto& cpuCntr : m_cpuCounters)
    {
      PDH_FMT_COUNTERVALUE counterVal;
      s = PdhGetFormattedCounterValue(cpuCntr, PDH_FMT_DOUBLE, NULL, &counterVal);
      values.push_back(counterVal.doubleValue);
    }

    return values;
  }

  CpuUsageSampler::~CpuUsageSampler()
  {
    PdhCloseQuery(m_cpuQuery);
  }
}