#pragma once

#include <string>

#ifdef _WIN32
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>
#endif // _WIN32

using namespace std;

class Limiter
{
    string app;

#ifdef _WIN32
    HANDLE hJob;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jLimits;
#endif // _WIN32



public:
    Limiter();

    bool setApp(string app);
    string getApp() const;

    bool setMinimumWorkingSetSize(unsigned int MB);
    unsigned int getMinimumWorkingSetSize() const;

    bool setMaximumWorkingSetSize(unsigned int MB);
    unsigned int getMaximumWorkingSetSize() const;

    bool setProcessMemoryLimit(unsigned int MB);
    unsigned int getProcessMemoryLimit() const;

    void applyLimits();
};
