#include <iostream>
#include <Windows.h>

typedef struct{
	int* a;
	int n;
	int min;
	int max;
	int mid;
} params;

DWORD WINAPI min_max(void* pr)
{
	params* arr = (params*)pr;
	arr->min = INT_MAX;
	arr->max = INT_MIN;
	for (int i = 0; i < arr->n; ++i)
	{
		if (arr->max < arr->a[i])
		{
			arr->max = arr->a[i];
		}
		Sleep(7);
		if (arr->min > arr->a[i])
		{
			arr->min = arr->a[i];
		}
		Sleep(7);
	}

	std::cout << "Maximal array element: " << arr->max << std::endl << "Minimal array element: " << arr->min << std::endl;

	ExitThread(0);
}

DWORD WINAPI average(void* pr)
{
	params* arr = (params*)pr;
	arr->mid = 0;
	for (int i = 0; i < arr->n; ++i)
	{
		arr->mid += arr->a[i];
		Sleep(12);
	}
	arr->mid /= arr->n;
	std::cout << "Arithmetic mean of the array: " << arr->mid << std::endl;

	ExitThread(0);
}

int main()
{
	HANDLE hThread1;
	DWORD IDThread1;
	HANDLE hThread2;
	DWORD IDThread2;

	std::cout << "Insert array size:\n";
	int n;
	std::cin >> n;

	int* a = new int[n];
	std::cout << "Insert array:\n";
	for (int i = 0; i < n; ++i)
	{
		std::cin >> a[i];
	}

	static params arr;
	arr.a = a;
	arr.n = n;

	if (!(hThread1 = CreateThread(NULL, 0, min_max, (void*)&arr, 0, &IDThread1)))
	{
		std::cout << "Thread is not created!\n";
		return GetLastError();
	}
	WaitForSingleObject(hThread1, INFINITE);

	if (!(hThread2 = CreateThread(NULL, 0, average, (void*)&arr, 0, &IDThread2)))
	{
		std::cout << "Thread is not created!\n";
		return GetLastError();
	}
	WaitForSingleObject(hThread2, INFINITE);

	for (int i = 0; i < n; ++i)
	{
		if (a[i] == arr.max)
		{
			a[i] = arr.mid;
		}
		if (a[i] == arr.min)
		{
			a[i] = arr.mid;
		}
		std::cout << a[i] << " ";
	}

	return 0;
}
