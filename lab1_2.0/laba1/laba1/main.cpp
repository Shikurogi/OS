#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <process.h>
#include <Windows.h>
#include <conio.h>
#include <fstream>

struct employee
{
	int num;
	char name[10];
	double hours;
};

int main() {

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	std::string fileName;
	std::cout << "Input file name:" << std::endl;
	std::cin >> fileName;
	std::cout << "Number of calls:" << std::endl;
	int n;
	std::cin >> n;

	std::string fileName1 = fileName + " " + std::to_string(n);
	std::string sCreator = "Creator.exe";
	sCreator = sCreator + " " + fileName1;
	const char* Creator = sCreator.c_str();
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, Creator, -1, wString, 4096);

	if (CreateProcess(NULL, wString, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		std::ifstream in;
		in.open(fileName, std::fstream::binary);
		employee* arr = new employee[n];

		for (int i = 0; i < n; i++) {
			in.read((char*)&arr[i], sizeof(employee));
		}

		for (int i = 0; i < n; i++) {
			std::cout << arr[i].num << " " << arr[i].name << " " << arr[i].hours << std::endl;
		}
	}

	std::string fileName2;
	std::cout << "Output file:" << std::endl;
	std::cin >> fileName2;
	int sum;
	std::cout << "Payment per hour:" << std::endl;
	std::cin >> sum;
	std::string sReporter = "Reporter.exe";
	sReporter += " " + fileName + " " + fileName2 + " " + std::to_string(sum);

	const char* Reporter = sReporter.c_str();
	wchar_t* wString1 = new wchar_t[4096];
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	MultiByteToWideChar(CP_ACP, 0, Reporter, -1, wString1, 4096);

	if (CreateProcess(NULL, wString1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		std::ifstream in1;
		in1.open(fileName2);
		std::string nameofthefile;
		in1 >> nameofthefile;
		std::cout << nameofthefile << std::endl;

		for (int i = 0; i < n; i++) {
			employee temp;
			in1 >> temp.num >> temp.name >> temp.hours;
			double salary;
			in1 >> salary;
			std::cout << temp.num << " " << temp.name << " " << temp.hours << " " << salary << std::endl;
		}
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}