#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex                       g_mutex;
std::vector<std::exception_ptr>  g_exceptions;


void min_max(int* a, int n, int& min, int& max)
{
	try
	{
		for (int i = 0; i < n; ++i)
		{
			if (min > a[i])
			{
				min = a[i];
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(7));
			if (max < a[i])
			{
				max = a[i];
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(7));
		}

		std::cout << "Maximal array element: " << max << std::endl << "Minimal array element: " << min << std::endl;

	}
	catch (...)
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		g_exceptions.push_back(std::current_exception());
	}
}

void average(int* a, int n, int& mid)
{
	try
	{
		for (int i = 0; i < n; ++i)
		{
			mid += a[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(12));
		}
		mid /= n;

		std::cout << "Arithmetic mean of the array: " << mid << std::endl;

	}
	catch (...)
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		g_exceptions.push_back(std::current_exception());
	}
}

int main()
{
	std::cout << "Input array size:\n";
	int n;		std::cin >> n;

	std::cout << "Input array:\n";
	int* a = new int[n];
	for (int i = 0; i < n; ++i)
	{
			std::cin >> a[i];
	}

	int min = INT_MAX;
	int max = INT_MIN;
	int mid = 0;

	std::thread thr1(min_max, a, n, std::ref(min), std::ref(max));
	thr1.join();

	std::thread thr2(average, a, n, std::ref(mid));
	thr2.join();

	for (auto& e : g_exceptions)
	{
		try
		{
			if (e != nullptr)
				std::rethrow_exception(e);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	std::cout << "New array: ";
	for (int i = 0; i < n; ++i)
	{
		if (a[i] == min)
		{
			a[i] = mid;
		}
		if (a[i] == max)
		{
			a[i] = mid;
		}

		std::cout << a[i] << " ";
	}
	
	return 0;
}