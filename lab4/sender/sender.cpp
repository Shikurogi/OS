#include <iostream>
#include <Windows.h>
#include <fstream>

CRITICAL_SECTION cs;

int main(int argc, char** argv) {

    InitializeCriticalSection(&cs);
    char* fileName = new char[4096];
    char* eventName = new char[4096];

    HANDLE event = OpenEvent(EVENT_MODIFY_STATE, FALSE, fileName);
    HANDLE synchronizedEvent = OpenEvent(SYNCHRONIZE, FALSE, eventName);
    std::ofstream out(argv[1], std::ofstream::binary|std::ios_base::app);

    SignalObjectAndWait(event, synchronizedEvent, INFINITE, FALSE);

    std::string msg;
    while (true) {
        std::cout << "Input message or input \"stop\" to stop the program:";
        std::cin >> msg;
        if (msg == "stop") {
            break;
        }
        EnterCriticalSection(&cs);
        msg += "\n";
        out.write((const char*)&msg, sizeof(std::string));
        SetEvent(event);
        LeaveCriticalSection(&cs);
    }

    CloseHandle(event);
    CloseHandle(synchronizedEvent);
    DeleteCriticalSection(&cs);
    out.close();
    return 0;
}
