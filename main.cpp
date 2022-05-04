#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define _WIN32_WINNT 0x0500

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#include <iostream>
using namespace std;

#include "include/limiter.h"

#define BAD_PARAMETERS 1


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure ( HWND, UINT, WPARAM, LPARAM );

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T ( "CodeBlocksWindowsApp" );

int WINAPI WinMain ( HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow )
{	
    Limiter algo;
    algo.addApp("c:\\autorun.inf\\a.ex_");
    algo.addApp("c:\\autorun.inf");
    algo.addApp("c:\\autorun.inf\\b.ex_");
    algo.setProcessMemoryLimit(3096);

    algo.setMaximumWorkingSetSize(2048);
    algo.setMinimumWorkingSetSize(1);

    algo.applyLimits();
    algo.run();

    /*
    // + Get configuration from Limiter.ini
    TCHAR MyDir[MAX_PATH]= {0};
    GetCurrentDirectory(MAX_PATH, MyDir);

    TCHAR IniPath[MAX_PATH]= {0};

    lstrcat (IniPath, MyDir);
    lstrcat (IniPath, _T("\\Limiter.ini"));

    TCHAR cApp[MAX_PATH]= {0};
    TCHAR cWorkDir[MAX_PATH]= {0};
    unsigned long long MinimumWorkingSetSize, MaximumWorkingSetSize, ProcessMemoryLimit;

    GetPrivateProfileString(_T("Global"), _T("Exe"), _T(""), cApp, sizeof(cApp), IniPath);
    auto aaaaa = GetLastError();
    GetPrivateProfileString(_TEXT("Global"), _TEXT("WorkDir"), _TEXT(""), cWorkDir, sizeof(cWorkDir), IniPath);
    MinimumWorkingSetSize= GetPrivateProfileInt(_T("Global"), _T("MinimumWorkingSetSize"), 0, IniPath);
    MaximumWorkingSetSize= GetPrivateProfileInt(_T("Global"), _T("MaximumWorkingSetSize"), 0, IniPath);
    ProcessMemoryLimit= GetPrivateProfileInt(_T("Global"), _T("ProcessMemoryLimit"), 0, IniPath);

    unsigned long long CheckMiliSec, PauseMiliSec;
    CheckMiliSec= GetPrivateProfileInt(_T("Global"), _T("CheckMiliSec"), 1000, IniPath);
    PauseMiliSec= GetPrivateProfileInt(_T("Global"), _T("PauseMiliSec"), 1, IniPath);
	// - Get configuration from Limiter.ini

    // If something when wrong abort
    if ( (lstrcmp(cApp, _T("")) == 0) | (MaximumWorkingSetSize == 0) | (ProcessMemoryLimit == 0))
        return BAD_PARAMETERS;

    // Convert from Mbytes to bytes
	MinimumWorkingSetSize *= 1024 * 1024;
    MaximumWorkingSetSize *= 1024 * 1024;
    ProcessMemoryLimit *= 1024 * 1024;
	
    // Create Job to impose limits
	HANDLE hJob = CreateJobObject ( NULL, TEXT("p667") );

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jLimits = {0};

    jLimits.ProcessMemoryLimit = ProcessMemoryLimit;
    jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;

    jLimits.BasicLimitInformation.MinimumWorkingSetSize = MinimumWorkingSetSize;
    jLimits.BasicLimitInformation.MaximumWorkingSetSize = MaximumWorkingSetSize;
    jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
    */


    /*   // Set limits to Job
	SetInformationJobObject ( hJob, JobObjectExtendedLimitInformation, &jLimits, sizeof ( jLimits ) );

    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof ( si );
    PROCESS_INFORMATION pi;

    CreateProcess ( NULL, cApp, NULL, NULL, FALSE,
                    CREATE_SUSPENDED, NULL, cWorkDir, &si, &pi );

    AssignProcessToJobObject ( hJob, pi.hProcess );
    printf("PID: %d\n", pi.dwProcessId);
    ResumeThread(pi.hThread);

    int exitWhile = 1;
    while (exitWhile)
    {
        HANDLE snap;

        // + See if process is over ...
		DWORD waitCode = WaitForSingleObject(pi.hProcess, CheckMiliSec);
        switch ( waitCode )
        {
        case WAIT_TIMEOUT:
            printf("Time out & running ...\n");
            break;
        case WAIT_OBJECT_0:
            printf("Ended !!!\n");
            exitWhile = 0;
            break;
        }
		// - See if process is over ...

        // Suspend process
		snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pi.dwProcessId);
        if (snap)
        {
            THREADENTRY32 threadEntry;
            threadEntry.dwSize = sizeof(THREADENTRY32);
            BOOL tOk = Thread32First(snap, &threadEntry);
            for(; tOk; tOk = Thread32Next(snap, &threadEntry))
            {
                if(threadEntry.th32OwnerProcessID == pi.dwProcessId)
                {
                    printf("\tSuspend: %d\n", threadEntry.th32ThreadID);

                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                    if (hThread)
                    {
                        SuspendThread(hThread);
                    }
                }
            }
            CloseHandle(snap);
        }
		
        // Wait for the next check
		Sleep(PauseMiliSec);
		
        // Resume process
		snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pi.dwProcessId);
        if (snap)
        {
            THREADENTRY32 threadEntry;
            threadEntry.dwSize = sizeof(THREADENTRY32);
            BOOL tOk = Thread32First(snap, &threadEntry);
            for(; tOk; tOk = Thread32Next(snap, &threadEntry))
            {
                if(threadEntry.th32OwnerProcessID == pi.dwProcessId)
                {
                    printf("\tResume: %d\n", threadEntry.th32ThreadID);

                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                    if (hThread)
                    {
                        ResumeThread(hThread);
                    }
                }
            }
            CloseHandle(snap);
        }
    }
    CloseHandle ( pi.hThread );

    CloseHandle ( pi.hProcess );
    CloseHandle ( hJob );*/

    return 0;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )                /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage ( 0 );     /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc ( hwnd, message, wParam, lParam );
    }

    return 0;
}
