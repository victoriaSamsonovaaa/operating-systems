#include <conio.h>
#include <windows.h>
#include <iostream>

struct employee
{
    int num; 
    char name[10];
    double hours;
};

void error()
{
    std::cout << "Oops! Error: " << GetLastError();
}

int main(int argc, char* argv) 
{

    HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
    if (hStartEvent == NULL) { error();return 0; }
    SetEvent(hStartEvent);

    HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\server", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) { error(); return 0; }

    while (true)
    {
        int option = 0;
        std::cout << "Choose option:\n1. Change record\n2. Read record\n3. Exit\n";
        std::cin >> option;
        switch (option)
        {
            case 1:
            {
                DWORD dwBytesWritten;
                DWORD dwBytesReaden;
                int ID, sendingMessage;

                std::cout << "Input employee's ID: ";
                std::cin >> ID;

                sendingMessage = ID * 10 + option;
                WriteFile(hPipe, &sendingMessage, sizeof(sendingMessage), &dwBytesWritten, NULL);

                employee* object = new employee();
                ReadFile(hPipe, object, sizeof(employee), &dwBytesReaden, NULL);

                std::cout << "\nEmployee" << "\nID: " << object->num << "\nName: " << object->name << "\nHours: " << object->hours << "\n";
                std::cout << "Input new employee's info (name, hours): ";
                std::cin >> object->name >> object->hours;
                WriteFile(hPipe, object, sizeof(employee), &dwBytesWritten, NULL);

                sendingMessage = 1;
                WriteFile(hPipe, &sendingMessage, sizeof(sendingMessage), &dwBytesWritten, NULL);
                break;
            }
            case 2:
            {
                DWORD dwBytesWritten;
                DWORD dwBytesReaden;
                int ID, sendingMessage;

                std::cout << "Input employee's ID: ";
                std::cin >> ID;

                sendingMessage = ID * 10 + option;
                WriteFile(hPipe, &sendingMessage, sizeof(sendingMessage), &dwBytesWritten, NULL);

                employee* object = new employee();
                ReadFile(hPipe, object, sizeof(employee), &dwBytesReaden, NULL);
                std::cout << "\nID: " << object->num << "\nName: " << object->name << "\nHours: " << object->hours << "\n";

                sendingMessage = 1;
                WriteFile(hPipe, &sendingMessage, sizeof(sendingMessage), &dwBytesWritten, NULL);
                break;
            }
            default:
            {
                _getch();
                return 0;
            }
        }
    }

    return 0;
}
