#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Employee
{
    int num;
    char name[10]; 
    double hours;
};

int main(int argc, const char* argv[])
{
    string binaryFileName = argv[1];
    int n = stoi(argv[2]);
    ofstream fout(binaryFileName, ios::binary);
    if (!fout.is_open())
    {
        std::cerr << "File error." << std::endl;
        return 1;
    }
    cout << "Input information about employee (ID, name, amount of hours):\n";
    for (int i = 0; i < n; ++i)
    {
        Employee employee;
        cin >> employee.num >> employee.name >> employee.hours;
        fout << employee.num << ' ' << employee.name << ' ' << employee.hours << '\n';
    }
    fout.close();
    cout << "Binary file was created\n";
    return 0;
}





		
		
