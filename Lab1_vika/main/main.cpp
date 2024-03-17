#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    std::cout << "Enter binary file name: ";
    std::string binaryFileName;
    std::cin >> binaryFileName;
    std::cout << std::endl << "Enter amount of records: ";
    int n;
    cin >> n;
    STARTUPINFO si;
    PROCESS_INFORMATION creatorPI;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    string consoleLine = "Creator.exe " + binaryFileName + ' ' + to_string(n);
    int len1 = strlen(consoleLine.c_str()) + 1;
    int size1 = MultiByteToWideChar(CP_UTF8, 0, consoleLine.c_str(), len1, NULL, 0);
    wchar_t* wConsoleName = new wchar_t[size1];
    MultiByteToWideChar(CP_UTF8, 0, consoleLine.c_str(), len1, wConsoleName, size1);
    // создаем новый процесс Creator.exe
    if (!CreateProcess(NULL, wConsoleName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &creatorPI))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }
    _cputs("The new process is created.\n");
    
    WaitForSingleObject(creatorPI.hProcess, INFINITE);
   
    CloseHandle(creatorPI.hThread);
    CloseHandle(creatorPI.hProcess);
   
    ifstream fin(binaryFileName);
    string str;
    while (getline(fin, str))
        cout << str << '\n';
    fin.close();

    std::cout << "Enter the name of report file: ";
    std::string reportFileName;
    std::cin >> reportFileName;
    std::cout << "Enter payment for an hour: ";
    double price;
    std::cin >> price;

    PROCESS_INFORMATION reporterPI;
    consoleLine = "Reporter.exe " + binaryFileName + ' ' + reportFileName + ' ' + to_string(price);
 
    len1 = strlen(consoleLine.c_str()) + 1;
    size1 = MultiByteToWideChar(CP_UTF8, 0, consoleLine.c_str(), len1, NULL, 0);
    wConsoleName = new wchar_t[size1];
    MultiByteToWideChar(CP_UTF8, 0, consoleLine.c_str(), len1, wConsoleName, size1);
    // создаем новый процесс Reporter.exe
    if (!CreateProcess(NULL, wConsoleName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &reporterPI))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }
    WaitForSingleObject(reporterPI.hProcess, INFINITE);
    
    CloseHandle(reporterPI.hThread);
    CloseHandle(reporterPI.hProcess);
    
    fin.open(reportFileName);
    while (getline(fin, str))
        cout << str << '\n';
    fin.close();
    return 0;
}