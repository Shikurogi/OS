#include <iostream>
#include <Windows.h>

typedef struct{
	int* a;
	int n;
} params;

volatile int min = INT_MAX;
volatile int max = INT_MIN;
volatile int mid = 0;

DWORD WINAPI min_max(void* pr)
{
	params* arr = (params*)pr;
	for (int i = 0; i < arr->n; ++i)
	{
		if (max < arr->a[i])
		{
			max = arr->a[i];
		}
		Sleep(7);
		if (min > arr->a[i])
		{
			min = arr->a[i];
		}
		Sleep(7);
	}

	std::cout << "Maximal array element: " << max << std::endl << "Minimal array element: " << min << std::endl;

	ExitThread(0);
}

DWORD WINAPI average(void* pr)
{
	params* arr = (params*)pr;
	for (int i = 0; i < arr->n; ++i)
	{
		mid += arr->a[i];
		Sleep(12);
	}
	mid /= arr->n;
	std::cout << "Arithmetic mean of the array: " << mid << std::endl;

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
		if (a[i] == max)
		{
			a[i] = mid;
		}
		if (a[i] == min)
		{
			a[i] = mid;
		}
		std::cout << a[i] << " ";
	}

	return 0;
}