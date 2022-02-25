#include <iostream>
#include <processthreadsapi.h>
#include <tchar.h>
#include <winbase.h>

int main() {
    std::cout << "Insert file name and employee number:" << std::endl;
    char fileName[15];
    int num;
    std::cin >> fileName >> num;

    TCHAR szPath[] = _T("D:\\Program Files\\CLion\\OS\\task1_Creator\\cmake-build-debug\\task1_Creator.exe");
    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(pi));
    if ( CreateProcess(
            NULL,
            szPath,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
// программа запущена, ждем её завершения

        DWORD dwWait = WaitForSingleObject(pi.hProcess, INFINITE);
        if ( dwWait == WAIT_OBJECT_0 )
        {
// программа благополучно завершилась

        }
        else if ( dwWait == WAIT_ABANDONED )
        {
// программа была насильно "прибита"

        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}
