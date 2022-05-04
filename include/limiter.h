#pragma once

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sys/stat.h>

#ifdef _WIN32
    #if defined(UNICODE) && !defined(_UNICODE)
        #define _UNICODE
    #elif defined(_UNICODE) && !defined(UNICODE)
        #define UNICODE
    #endif

	#include <tchar.h>
	#include <windows.h>
	#include <tlhelp32.h>
#endif // _WIN32

using namespace std;

class Limiter
{
    vector<string> apps;

#ifdef _WIN32
    HANDLE hJob;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jLimits;

    vector<STARTUPINFO> v_si;
    vector<PROCESS_INFORMATION> v_pi;
#endif // _WIN32



public:
    Limiter();

    bool addApp(string app);
    string getApp(unsigned int index) const;

    bool setMinimumWorkingSetSize(unsigned long MB);
    unsigned long getMinimumWorkingSetSize() const;

    bool setMaximumWorkingSetSize(unsigned long MB);
    unsigned long getMaximumWorkingSetSize() const;

    bool setProcessMemoryLimit(unsigned long MB);
    unsigned long getProcessMemoryLimit() const;

    void applyLimits();
    void run();
};
