#include <iostream>
#include <windows.h>
#include <conio.h>
#include "employee.h"

const char pipeName[50] = "\\\\.\\pipe\\demo_pipe";

void requests(HANDLE hPipe){
    while(true){
        char cmd[15];
        std::cout << "Enter \'r\' to read or \'w\' to write and the id of the employee:\n";
        std::cin.getline(cmd, 15, '\n');
        DWORD dwBytesRead;
        if (!(WriteFile(hPipe, cmd, 15, &dwBytesRead, NULL))){
            std::cout << GetLastError() << std::endl;
            return;
        }
        employee e;
        DWORD readBytes;
        if (!(ReadFile(hPipe, &e, sizeof(e), &readBytes, NULL))){
            std::cout << GetLastError() << std::endl;
        }
        else{
            if(e.num < 0) {
                std::cout << "Employee not found or being modified." << std::endl;
                continue;
            }
            else {
                e.output(std::cout);
                if('w' == cmd[0]) {
                    std::cout << "Enter employee id, name and the amount of working hours:\n";
                    std::cin >> e.num >> e.name >> e.hours;
                    std::cin.ignore(2, '\n');
                    if (!(WriteFile(hPipe, &e, sizeof (e), &dwBytesRead, NULL)))
                    {
                        std::cout << GetLastError() << std::endl;
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    HANDLE hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE hStartEvent = OpenEvent(SYNCHRONIZE, FALSE, "START_ALL");
    if(hStartEvent == NULL || hReadyEvent == NULL){
        return GetLastError();
    }
    SetEvent(hReadyEvent);
    WaitForSingleObject(hStartEvent, INFINITE);

    HANDLE hNamedPipe;
    while(true) {
        hNamedPipe = CreateFile(pipeName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE != hNamedPipe) {
            break;
        }
        if(!WaitNamedPipe(pipeName, 5000)){
            return 0;
        }
    }
    requests(hNamedPipe);
    return 0;
}