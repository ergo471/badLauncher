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

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure ( HWND, UINT, WPARAM, LPARAM );

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T ( "CodeBlocksWindowsApp" );

int WINAPI WinMain ( HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow )
{
    char MyDir[MAX_PATH]= {0};
    GetCurrentDirectory(MAX_PATH, MyDir);

    char IniPath[MAX_PATH]= {0};
    strcat(IniPath, MyDir);
    strcat(IniPath, "\\Limiter.ini");

    char cApp[MAX_PATH]= {0};
    char cWorkDir[MAX_PATH]= {0};
    unsigned long long MinimumWorkingSetSize, MaximumWorkingSetSize, ProcessMemoryLimit;

    GetPrivateProfileString("Global", "Exe", "", cApp, sizeof(cApp), IniPath);
    GetPrivateProfileString("Global", "WorkDir", "", cWorkDir, sizeof(cWorkDir), IniPath);
    MinimumWorkingSetSize= GetPrivateProfileInt("Global", "MinimumWorkingSetSize", 0, IniPath);
    MaximumWorkingSetSize= GetPrivateProfileInt("Global", "MaximumWorkingSetSize", 0, IniPath);
    ProcessMemoryLimit= GetPrivateProfileInt("Global", "ProcessMemoryLimit", 0, IniPath);

    unsigned long long CheckMiliSec, PauseMiliSec;
    CheckMiliSec= GetPrivateProfileInt("Global", "CheckMiliSec", 1000, IniPath);
    PauseMiliSec= GetPrivateProfileInt("Global", "PauseMiliSec", 1, IniPath);

    if ( (strcmp(cApp, "") == 0) | (MaximumWorkingSetSize == 0) | (ProcessMemoryLimit == 0))
        return 0;

    MinimumWorkingSetSize *= 1024 * 1024;
    MaximumWorkingSetSize *= 1024 * 1024;
    ProcessMemoryLimit *= 1024 * 1024;


    HANDLE hJob = CreateJobObject ( NULL, TEXT("p667") );

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jLimits = {0};

    jLimits.ProcessMemoryLimit = ProcessMemoryLimit;
    jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;

    jLimits.BasicLimitInformation.MinimumWorkingSetSize = MinimumWorkingSetSize;
    jLimits.BasicLimitInformation.MaximumWorkingSetSize = MaximumWorkingSetSize;
    jLimits.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;

    SetInformationJobObject ( hJob, JobObjectExtendedLimitInformation, &jLimits, sizeof ( jLimits ) );

    STARTUPINFO si = { sizeof ( si ) };
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
        Sleep(PauseMiliSec);
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
    CloseHandle ( hJob );

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
