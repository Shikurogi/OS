#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include "employee.h"

int n;
employee* employees;
HANDLE* hReadyEvents;
CRITICAL_SECTION cs;
std::vector<bool> check;
const char pipeName[50] = "\\\\.\\pipe\\demo_pipe";

void createProcesses(int count){
    char evNum[10];
    for(int i = 0; i < count; ++i) {
        std::string args = "..\\..\\Client\\cmake-build-debug\\Client.exe ";
        std::string evName = "READY_EVENT_";
        itoa(i + 1, evNum, 10);
        evName += evNum;
        args += evName;
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        char wArgs[4096];
        char wEvName[4096];
        strcpy(wArgs, args.c_str());
        strcpy(wEvName, evName.c_str());
        hReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, wEvName);
        if (!CreateProcess(NULL, wArgs, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,NULL, NULL, &si, &pi)) {
            std::cout << GetLastError() << std::endl;
            return;
        }
    }
}

employee* empSearch(int id){
    employee key;
    key.num = id;
    return (employee*)bsearch((const char*)(&key), (const char*)(employees), n, sizeof(employee), comp);
}

DWORD WINAPI requests(void* pr) {
    HANDLE hNamedPipe = (HANDLE) pr;
    employee *error = new employee;
    error->num = -1;
    while (true) {
        DWORD dwBytesRead;
        char request[15];
        bool isRead = ReadFile(hNamedPipe, request, 15, &dwBytesRead, NULL);
        if (!isRead) {
            std::cout << GetLastError() << std::endl;
            break;
        }
        if (strlen(request) > 0) {
            char cmd = request[0];
            request[0] = ' ';
            int id = atoi(request);
            DWORD bytesWritten;
            EnterCriticalSection(&cs);
            employee *tmp = empSearch(id);
            LeaveCriticalSection(&cs);
            if (tmp == NULL) {
                tmp = error;
            } else {
                int index = tmp - employees;
                if (check[index])
                    tmp = error;
                else {
                    if (cmd == 'w') {
                        std::cout << "id: " << id << " Requests to record the data.";
                        check[index] = true;
                        break;
                    } else if (cmd == 'r') {
                        std::cout << "id: " << id << " Requests to read the data.";
                        break;
                    } else {
                        std::cout << "Invalid request.";
                        tmp = error;
                    }
                }
            }
            if (!(WriteFile(hNamedPipe, tmp, sizeof(employee), &bytesWritten, NULL))) {
                std::cout << "An error occurred during the request execution!";
                return GetLastError();
            }
            if (cmd == 'w' && tmp != error) {
                if (!(ReadFile(hNamedPipe, tmp, sizeof(employee), &dwBytesRead, NULL))){
                    std::cerr << "Invalid request.\n";
                    break;
                } else {
                    check[tmp - employees] = false;
                    EnterCriticalSection(&cs);
                    qsort(employees, n, sizeof(employee), comp);
                    LeaveCriticalSection(&cs);
                }
            }
        }
    }
    CloseHandle(hNamedPipe);
    delete error;
    return 0;
}

void createPipes(int m) {
    HANDLE hNamedPipe;
    HANDLE *hThreads = new HANDLE[m];
    for (int i = 0; i < m; ++i) {
        if (!(hNamedPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                           PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL))) {
            std::cerr << GetLastError() << std::endl;
            return;
        }
        if (!ConnectNamedPipe(hNamedPipe, NULL)) {
            std::cout << GetLastError() << std::endl;
            return;
        }
        hThreads[i] = CreateThread(NULL, 0, requests, (void *) hNamedPipe, 0, NULL);
    }
    WaitForMultipleObjects(m, hThreads, TRUE, INFINITE);
    delete[] hThreads;
}

int main() {
    char filename[15];
    std::cout << "Enter the file name and the number of employees:\n";
    std::cin >> filename >> n;
    employees = new employee[n];
    std::cout << "Enter employees data:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> employees[i].num >> employees[i].name >> employees[i].hours;
    }

    std::fstream out(filename, std::ios::binary | std::ios::out);
    out.write((const char *) employees, sizeof(employees) * n);
    qsort(employees, n, sizeof(employee), comp);

    std::cout << "Enter the number of clients:\n";
    int clientNum;
    std::cin >> clientNum;
    InitializeCriticalSection(&cs);
    srand(time(0));
    HANDLE hStartALL = CreateEvent(NULL, TRUE, FALSE, "START_ALL");
    check.resize(n, false);
    hReadyEvents = new HANDLE[clientNum];
    createProcesses(clientNum);
    WaitForMultipleObjects(clientNum, hReadyEvents, TRUE, INFINITE);
    SetEvent(hStartALL);

    createPipes(clientNum);
    for (int i = 0; i < n; ++i)
        employees[i].output(std::cout);

    DeleteCriticalSection(&cs);
    delete[] hReadyEvents;
    delete[] employees;
    return 0;
}