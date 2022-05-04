#include "../include/limiter.h"

Limiter::Limiter()
{
#ifdef _WIN32
    hJob = CreateJobObject ( NULL, NULL );

    memset(&jLimits, 0, sizeof(jLimits));
#endif // _WIN32
}

bool Limiter::addApp(string app)
{
    // Check that app exist using C++ std functions
    struct stat stbuf;
    stat(app.c_str(), &stbuf);

    if(not(stbuf.st_mode & S_IFREG))
        return false;

    apps.push_back(app);

    return true;
}

string Limiter::getApp(unsigned int index) const
{
    return apps.at(index);
}

bool Limiter::setMinimumWorkingSetSize(unsigned long MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    if ( (MB > 0) && (MB * 1024*1024 <= jLimits.BasicLimitInformation.MaximumWorkingSetSize) )
    {
        jLimits.BasicLimitInformation.MinimumWorkingSetSize = MB * 1024*1024;
        jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
    }
    else
        return false;
#endif // _WIN32

    return true;
}

unsigned long Limiter::getMinimumWorkingSetSize() const
{
#ifdef _WIN32
    return jLimits.BasicLimitInformation.MinimumWorkingSetSize;
#endif // _WIN32
}

bool Limiter::setMaximumWorkingSetSize(unsigned long MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    if ( (MB > 0) && (MB * 1024*1024 >= jLimits.BasicLimitInformation.MinimumWorkingSetSize) )
    {
        jLimits.BasicLimitInformation.MaximumWorkingSetSize = MB * 1024*1024;
        jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
    }
    else
        return false;
#endif // _WIN32

    return true;
}

unsigned long Limiter::getMaximumWorkingSetSize() const
{
#ifdef _WIN32
    return jLimits.BasicLimitInformation.MaximumWorkingSetSize;
#endif // _WIN32
}

bool Limiter::setProcessMemoryLimit(unsigned long MB)
{
#ifdef _WIN32
    // Code for Windows Platform
    unsigned long MAX_MB = 3uL *1024*1024*1024;
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

unsigned long Limiter::getProcessMemoryLimit() const
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

void Limiter::run()
{
#ifdef _WIN32
    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof ( si );
    PROCESS_INFORMATION pi;

    TCHAR buffer[MAX_PATH];
    for(size_t i=0; i < apps.size(); ++i)
    {
        v_si.push_back(si);
        v_pi.push_back(pi);

        wstring tmpWstring(apps[i].begin(), apps[i].end());
        wcscpy(buffer, tmpWstring.c_str());

        CreateProcess ( NULL, buffer, NULL, NULL, FALSE,
                        CREATE_SUSPENDED, NULL, L".", &v_si[i], &v_pi[i] );

        AssignProcessToJobObject ( hJob, v_pi[i].hProcess );

        ResumeThread(v_pi[i].hThread);
    }
#endif // _WIN32
}
