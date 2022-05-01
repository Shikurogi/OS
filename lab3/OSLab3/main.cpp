#include <iostream>
#include <time.h>
#include <windows.h>
#include <process.h>

using std::cout;
using std::cin;
using std::endl;

HANDLE* events;
CRITICAL_SECTION cs;
HANDLE workEvent, hOutEvent, hAddEvent;

struct  array{
	int* a;
	int n;
    int x;

    array()
    {

    }

    array(int* a0, int n0, int x0)
    {
        a = a0;
        n = n0;
        x = x0;
    }
};

DWORD WINAPI marker(void* pr)
{
	array* arr = (array*)pr;
    WaitForSingleObject(workEvent, INFINITE);
    srand(arr->x);
    cout << "Thread " << arr->x << " started.\n";

    int k = 0;
    while(true){
        EnterCriticalSection(&cs);
        int i = rand() % arr->n;
        if(arr->a[i] == 0){
            Sleep(5);
            arr->a[i] = arr->x;
            ++k;
            LeaveCriticalSection(&cs);
            Sleep(5);
        } else {
            LeaveCriticalSection(&cs);
            SetEvent(events[arr->x-1]);
            HANDLE tmp[2];
            tmp[0] = hOutEvent;
            tmp[1] = hAddEvent;
            int action = WaitForMultipleObjects(2, tmp, FALSE, INFINITE) - WAIT_OBJECT_0;
            if(action == 1){
                for(int j = 0; j < arr->n; j++){
                    if(arr->a[j] == arr->x){
                        arr->a[j] = 0;
                    }
                }
                cout << "Thread " << arr->x << " is stopped.";
                break;
            }
        }
    }
    return NULL;
}

int main()
{
	cout << "Enter array size:\n";
	int n;
	cin >> n;

	int* a = new int[n];
	for (int i = 0; i < n; ++i)
	{
		a[i] = 0;
	}

	int th;
	cout << "Enter the amount of threads:\n";
	cin >> th;

	HANDLE* hThread = new HANDLE[th];
	DWORD* dwThread = new DWORD[th];
    events = new HANDLE[th];
    bool* isWorking = new bool[th];
    array* arrays = new array[th];
    if (!(workEvent= CreateEvent(NULL, FALSE, FALSE, NULL)))
        return GetLastError();
    if (!(hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
        return GetLastError();
    if (!(hAddEvent  = CreateEvent(NULL, FALSE, FALSE, NULL)))
        return GetLastError();

	for (int i = 0; i < th; ++i)
	{
        array tmp(a, n, i);
		if (!(hThread[i] = CreateThread(NULL, 0, marker, (void*)&tmp, 0, &dwThread[i])))
		{
			std::cout << "Thread number " << i + 1 << " is not created!\n";
			return GetLastError();
		}
        isWorking[i] = false;
        events[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        arrays[i] = tmp;
	}

    InitializeCriticalSection(&cs);
    PulseEvent(workEvent);
    int i = 0, stop;
    while(i != th) {
        WaitForMultipleObjects(th, &events[0], TRUE, INFINITE);

        EnterCriticalSection(&cs);
        for(int j = 0; j < n; ++j){
            cout << a[j] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&cs);

        cout <<"The thread number that must be stopped:\n";
        cin >> stop;
        isWorking[stop-1] = true;
        SetEvent(hOutEvent);
        WaitForSingleObject(hThread[stop-1], INFINITE);
        ++i;

        EnterCriticalSection(&cs);
        for(int j = 0; j < n; ++j){
            cout << a[j] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&cs);

        for(int j = 0; j < th; ++j){
            if(isWorking[j])
                continue;
            ResetEvent(events[j]);
            SetEvent(hAddEvent);
        }
    }
    DeleteCriticalSection(&cs);
	return 0;
}