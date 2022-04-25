#include "../include/limiter.h"

Limiter::Limiter(): jLimits{0}
{
#ifdef _WIN32
    hJob = CreateJobObject ( NULL, NULL );
#endif // _WIN32
}

bool Limiter::setApp(string app)
{
    // Check that app exist using C++ std functions

    return true;
}

string Limiter::getApp() const
{
    return app;
}

bool Limiter::setMinimumWorkingSetSize(unsigned int MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    if (MB > 0)
    {
        jLimits.BasicLimitInformation.MinimumWorkingSetSize = MB * 1024*1024;
        jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
    }
    else
        return false;
#endif // _WIN32

    return true;
}

unsigned int Limiter::getMinimumWorkingSetSize() const
{
#ifdef _WIN32
    return jLimits.BasicLimitInformation.MinimumWorkingSetSize;
#endif // _WIN32
}

bool Limiter::setMaximumWorkingSetSize(unsigned int MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    if (MB > 0)
    {
        jLimits.BasicLimitInformation.MaximumWorkingSetSize = MB * 1024*1024;
        jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
    }
    else
        return false;
#endif // _WIN32

    return true;
}

unsigned int Limiter::getMaximumWorkingSetSize() const
{
#ifdef _WIN32
    return jLimits.BasicLimitInformation.MaximumWorkingSetSize;
#endif // _WIN32
}

bool Limiter::setProcessMemoryLimit(unsigned int MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    if (MB > 0)
    {
        jLimits.ProcessMemoryLimit = MB * 1024*1024;
        jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
    }
    else
        return false;
#endif // _WIN32

    return true;
}

unsigned int Limiter::getProcessMemoryLimit() const
{
#ifdef _WIN32
    return jLimits.ProcessMemoryLimit;
#endif // _WIN32
}

void Limiter::applyLimits()
{
#ifdef _WIN32
    SetInformationJobObject ( hJob, JobObjectExtendedLimitInformation, &jLimits, sizeof ( jLimits ) );
#endif // _WIN32
}
