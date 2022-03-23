#include <iostream>
#include <Windows.h>

using std::cout;
using std::cin;
using std::endl;

typedef struct {
	int* a;
	int n;
} array;

DWORD WINAPI marker(void* pr)
{
	array* arr = (array*)pr;

}

int main()
{
	cout << "Enter array size:\n";
	int n;
	cin >> n;

	cout << "Enter array:\n";
	int* a = new int[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
	}

	int th;
	cout << "Enter thread number:\n";
	cin >> th;

	HANDLE* hThread = new HANDLE[th];
	DWORD* dwThread = new DWORD[th];

	for (int i = 0; i < th; ++i)
	{
		if (!(hThread[i] = CreateThread(NULL, 0, marker, (void*)&a, 0, &dwThread[i])))
		{
			std::cout << "Thread number" << i + 1 << " is not created!\n";
			return GetLastError();
		}
	}

	for (int i = 0; i < n; ++i)
	{
		cout << a[i] << " ";
	}
	cout << endl;

	int k;
	cout << "Insert thread number that must be stopped:\n";
	cin >> k;
	SuspendThread(hThread);



	if (WaitForMultipleObjects(th, hThread, TRUE, INFINITE) == WAIT_FAILED)
	{
		cout << "Wait for multiple objects failed.\n";
		return GetLastError();
	}

	return 0;
}