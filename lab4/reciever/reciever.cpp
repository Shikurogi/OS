#include <iostream>
#include <Windows.h>
#include <fstream>

CRITICAL_SECTION cs;

int main() {
    InitializeCriticalSection(&cs);
    std::cout << "Insert filename:" << std::endl;
    std::string fileName;
    std::cin >> fileName;
    int num;
    std::cin >> num;

    HANDLE* synchronizedEvent = new HANDLE[num];
    HANDLE* senders = new HANDLE[num];
    STARTUPINFO* si = new STARTUPINFO[num];
    PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[num];
    SECURITY_ATTRIBUTES security = { sizeof(SECURITY_ATTRIBUTES), 0, TRUE };
    char* eve = "startEvent";
    HANDLE startEvent = CreateEvent(&security, FALSE, FALSE, eve);

    for (int i = 0; i < num; ++i) {
        std::string names = "sender.exe";
        names = names + " " + fileName;
        std::string evNames = "event";
        evNames = evNames + " " + std::to_string(i);
        char wEvNames[4096];
        strcpy(wEvNames, evNames.c_str());
        if(!(synchronizedEvent[i] = CreateEvent(&security, FALSE, FALSE, wEvNames))){
            std::cout << "Event " << i << " is not created!";
            return GetLastError();
        }
        names = names + " event" + std::to_string(i);
        char wNames[4096];
        strcpy(wNames, names.c_str());
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
        if (!CreateProcess(NULL, wNames, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i])) {
            std::cout << "Process is not created!";
            return GetLastError();
        }
        senders[i] = pi[i].hProcess;
    }
    WaitForMultipleObjects(num, synchronizedEvent, TRUE, INFINITE);
    SetEvent(startEvent);

    while (WaitForMultipleObjects(num, senders, TRUE, 0) == WAIT_TIMEOUT) {
        std::cout << "Input \"start\" for trying to read file: ";
        std::string text;
        std::cin >> text;
        if (text != "start") {
            break;
        }
        EnterCriticalSection(&cs);
        std::ifstream in(fileName, std::ifstream::binary);
        std::string msg;
        for (int i = 0; i < 2; ++i)
        {
            in.read((char*)&msg, sizeof(std::string));
        }
        in.close();
        LeaveCriticalSection(&cs);
    }

    DeleteCriticalSection(&cs);
    CloseHandle(startEvent);
    return 0;
}