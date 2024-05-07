#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <algorithm>

struct employee
{
	int num;
	char name[10];
	double hours;
};


HANDLE* hStarted;
HANDLE* hPipe;
HANDLE* hThreads;
HANDLE* hSemaphore;
employee* objects;
int number_of_employees;
int number_of_clients;
std::string file_name;
std::fstream file;
int id;
char name[10];
double hours;

void error();
void inputFile();
void initFile();
void outputFile();
void initClient();
DWORD WINAPI operations(LPVOID);

DWORD WINAPI operations(LPVOID pipe)
{
	HANDLE hPipe = (HANDLE)pipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;

	int message;
	while (true)
	{
		if (!ReadFile(hPipe, &message, sizeof(message), &dwBytesRead, NULL)) { error(); return NULL; }
		else
		{
			int ID = message / 10;
			int option = message % 10;

			switch (option % 10)
			{
			case 1:
			{
				for (int i = 0; i < number_of_clients; i++)
				{
					WaitForSingleObject(hSemaphore[ID - 1], INFINITE);
				}
				employee* emp_to_push = new employee();
				emp_to_push->num = objects[ID - 1].num;
				emp_to_push->hours = objects[ID - 1].hours;
				strcpy_s(emp_to_push->name, objects[ID - 1].name);

				WriteFile(hPipe, emp_to_push, sizeof(employee), &dwBytesWrite, NULL);
				ReadFile(hPipe, emp_to_push, sizeof(employee), &dwBytesWrite, NULL);

				objects[ID - 1].hours = emp_to_push->hours;
				strcpy_s(objects[ID - 1].name, emp_to_push->name);
				inputFile();
				int msg;
				ReadFile(hPipe, &msg, sizeof(msg), &dwBytesWrite, NULL);
				if (msg == 1) {
					for (int i = 0; i < number_of_clients; i++)
					{
						ReleaseSemaphore(hSemaphore[ID - 1], 1, NULL);
					}
				}
				break;
			}
			case 2:
			{
				WaitForSingleObject(hSemaphore[ID - 1], INFINITE);
				employee* emp_to_push = new employee();
				emp_to_push->num = objects[ID - 1].num;
				emp_to_push->hours = objects[ID - 1].hours;
				strcpy_s(emp_to_push->name, objects[ID - 1].name);
				WriteFile(hPipe, emp_to_push, sizeof(employee), &dwBytesWrite, NULL);
				int msg;
				ReadFile(hPipe, &msg, sizeof(msg), &dwBytesWrite, NULL);
				if (msg == 1)
					ReleaseSemaphore(hSemaphore[ID - 1], 1, NULL);
			}
			}
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

}

void error()
{
	if (GetLastError() == 109) { return; }
	std::cout << "Oops! Error: " << GetLastError();
}

void inputFile()
{
	file.open(file_name, std::ios::out);
	for (int i = 0; i < number_of_employees; i++)
	{
		file << objects[i].num << " " << objects[i].name << " " << objects[i].hours << "\n";
	}
	file.close();
}

void initFile()
{
	std::cout << "Input file name: ";
	std::cin >> file_name;
	std::cout << "Input number of employees: ";
	std::cin >> number_of_employees;

	objects = new employee[number_of_employees];
	for (int i = 0; i < number_of_employees; i++)
	{
		std::cout << "Input employee's data (id, name, hours): ";
		std::cin >> objects[i].num >> objects[i].name >> objects[i].hours;
	}

	inputFile();
}

void outputFile()
{
	file.open(file_name, std::ios::in);
	std::cout << "\n\n - - - File content - - - \n\n";
	for (int i = 0; i < number_of_employees; i++)
	{
		file >> id >> name >> hours;
		std::cout << "\nEmployee #" << (i + 1) << "\nID: " << id << "\nName: " << name << "\nHours: " << hours << "\n";
	}
	std::cout << "\n\n - - - File content - - - \n\n";
	file.close();
}

void initClient()
{
	std::cout << "Input number of clients: ";
	std::cin >> number_of_clients;

	hStarted = new HANDLE[number_of_clients];
	hSemaphore = new HANDLE[number_of_employees];
	for (int i = 0; i < number_of_employees; ++i)
	{
		hSemaphore[i] = CreateSemaphore(NULL, number_of_clients, number_of_clients, L"hSemahpore");
	}
	for (int i = 0; i < number_of_clients; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		std::string cmd = "Client.exe";
		std::wstring str_to_wstr = std::wstring(cmd.begin(), cmd.end());
		LPWSTR clientCmdLine = &str_to_wstr[0];
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		CreateProcess(NULL, clientCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		hStarted[i] = CreateEvent(NULL, FALSE, FALSE, L"Process Started");
		CloseHandle(pi.hProcess);
	}
	WaitForMultipleObjects(number_of_clients, hStarted, TRUE, INFINITE);

	hPipe = new HANDLE[number_of_clients];
	hThreads = new HANDLE[number_of_clients];
	for (int i = 0; i < number_of_clients; ++i)
	{
		hPipe[i] = CreateNamedPipe(L"\\\\.\\pipe\\server", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);

		if (hPipe == INVALID_HANDLE_VALUE) { error(); return;  }
		if (!ConnectNamedPipe(hPipe[i], (LPOVERLAPPED)NULL)) { error(); return; }

		hThreads[i] = CreateThread(NULL, 0, operations, static_cast<LPVOID>(hPipe[i]), 0, NULL);
	}
	WaitForMultipleObjects(number_of_clients, hThreads, TRUE, INFINITE);
}

int main()
{
	initFile();
	outputFile();
	initClient();
	outputFile();
}
